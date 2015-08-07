//    Copyright (c) 2010, Christopher James Huff <cjameshuff@gmail.com>
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions are met:
//        * Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//        * Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the distribution.
//        * Neither the name of the copyright holder nor the names of contributors
//          may be used to endorse or promote products derived from this software
//          without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
//    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
//************************************************************************************************

#include <util/delay.h>
#include <avr/io.h>

#include "shiftlcd.h"

//************************************************************************************************

static void CLCD_SetShiftReg(uint8_t state);
static void CLCD_SendByte(uint8_t byte);
static void CLCD_SendCommand(uint8_t command);

// LCD state
// CGRAM/DDRAM address
// Entry mode:
//       I/D: cursor move direction (Increment/Decrement)
//       S: Shift display
// Display control:
//       D: Display on/off
//       C: Cursor on/off
//       B: Blink on/off
// Cursor/display shift:
//     S/C: Shift display or move Cursor
//     R/L: shift direction
// Function set:
//     DL: Data Length (always 4 bit)
//     N: Number of display lines (compile constant)
//     F: character Font
//
// 8 bits of variable state to track

//static struct {
//    uint8_t cursorIncDec: 1;
//    uint8_t shiftDisp: 1;
//    uint8_t displayOn: 1;
//    uint8_t showCursor: 1;
//    uint8_t blinkCursor: 1;
//    uint8_t shiftDisp_Cursor: 1;
//    uint8_t shiftDir: 1;// right if set, left if clear
//    uint8_t charFont: 1;// 5x11 if set, 5x7 if clear
//} iflags;

//static uint8_t CLCD_addr;

#if(CLCD_ASYNC)
// High 2 bytes of CLCD_updateState are line number.
// Low 6 bytes of CLCD_updateState are line index of next character to send.
// Values 0..CLCD_LINE_WIDTH - 1 are used for character indexes, CLCD_LINE_WIDTH
// triggers a move to the next line. 0xFF triggers start of the first line.
// All state values > 0xE8 are available
#define CLCDSTATE_REFRESH_DONE (0xFF)
#define CLCDSTATE_START_REFRESH (0xFE)

static char * CLCD_buffer;
static uint8_t CLCD_updateState;
#endif // CLCD_ASYNC

static uint8_t CLCD_lcdState;

//------------------------------------------------------------------------------------------------

// CLCD commands
#define CMD_CLRDIS 0x01
#define SendClearDisplayCMD() CLCD_SendCommand(CMD_CLRDIS); CLCD_LONGDELAY()

#define CMD_HOME 0x02
#define SendCursorToHomeCMD() CLCD_SendCommand(CMD_HOME); CLCD_LONGDELAY()

#define CMD_SET_ENTRY_MODE 0x04
  #define CMDPARM_SEM_SHIFT_ON 0x01
  #define CMDPARM_SEM_SHIFT_OFF 0x00
  #define CMDPARM_SEM_CURSOR_LEFT 0x00
  #define CMDPARM_SEM_CURSOR_RIGHT 0x02
#define SendSetEntryModeCMD(params) CLCD_SendCommand(CMD_SET_ENTRY_MODE | params)

#define CMD_DISP_CONTROL 0x08
  #define CMDPARM_DC_BLINK_ON 0x01
  #define CMDPARM_DC_BLINK_OFF 0x00
  #define CMDPARM_DC_CURSOR_ON 0x02
  #define CMDPARM_DC_CURSOR_OFF 0x00
  #define CMDPARM_DC_DISP_ON 0x04
  #define CMDPARM_DC_DISP_OFF 0x00
#define SendDisplayControlCMD(params) CLCD_SendCommand(CMD_DISP_CONTROL | params)

#define CMD_CURS_DIS_SHIFT 0x10
  #define CMDPARM_CDS_RIGHT 0x04
  #define CMDPARM_CDS_LEFT 0x00
  #define CMDPARM_CDS_DISPLAY 0x08
  #define CMDPARM_CDS_CURSOR 0x00
#define SendCursorDisplayShiftCMD(params) CLCD_SendCommand(CMD_CURS_DIS_SHIFT | params)

#define CMD_SETFUNC 0x20
  #define CMDPARM_SF_5x11 0x04
  #define CMDPARM_SF_5x8 0x00
  #define CMDPARM_SF_2_LINE 0x08
  #define CMDPARM_SF_1_LINE 0x00
  #define CMDPARM_SF_8_BIT 0x10
  #define CMDPARM_SF_4_BIT 0x00
#define SendSetFunctionCMD(params) CLCD_SendCommand(CMD_SETFUNC | params)


#define CMD_S_CGRAM_ADDR 0x40
#define CMD_S_DDRAM_ADDR 0x80


// set CMD/CHAR bit to return to character/data mode
#define LCD_CTRL_SetCHAR() CLCD_lcdState |= LCD_CTRL_CMD_CHAR

// clear CMD/CHAR bit to select command mode
#define LCD_CTRL_SetCMD() CLCD_lcdState &= ~LCD_CTRL_CMD_CHAR



//************************************************************************************************

void CLCD_Init(void)
{
    CLCD_CS_DDR |= CLCD_CS;
    CLCD_CS_PORT |= CLCD_CS;
    
    CLCD_SetShiftReg(0x00);
    delay_ms(100);
    
#if(CLCD_ASYNC)
    CLCD_updateState = CLCDSTATE_REFRESH_DONE;
#endif // CLCD_ASYNC
    
    // All LCDs simultaneously active for initialization.
    // Set CMD/CHAR bit to set character/data mode. Driver normally remains in this mode,
    // only switching to command mode temporarily for individual commands.
    CLCD_lcdState = CLCD_ALL_LCDS;
    LCD_CTRL_SetCHAR();
    
    // Set 4-bit mode. Display is in 8 bit mode for the first command sent, which
    // requires special handling.
    // Set command, high nybble
    uint8_t portState = CMD_SETFUNC | CLCD_ALL_LCDS;
    portState |= CLCD_lcdState;
    CLCD_SetShiftReg(portState);
    CLCD_DELAY();
    
    // Set enable low, latch high nybble
    portState &= ~(CLCD_lcdState & CLCD_ALL_LCDS);
    CLCD_SetShiftReg(portState);
    
    // Repeat
    portState |= CLCD_lcdState;
    CLCD_SetShiftReg(portState);
    CLCD_DELAY();
    
    // Set enable low, latch high nybble
    portState &= ~(CLCD_lcdState & CLCD_ALL_LCDS);
    CLCD_SetShiftReg(portState);
    
    CLCD_DELAY();
    
    // Display is now in 4 bit mode. Set up display.
#if(CLCD_LINES == 1)
    SendSetFunctionCMD(CMDPARM_SF_4_BIT | CMDPARM_SF_1_LINE | CMDPARM_SF_5x8);
#else
    SendSetFunctionCMD(CMDPARM_SF_4_BIT | CMDPARM_SF_2_LINE | CMDPARM_SF_5x8);
#endif
    CLCD_DELAY();
    
    SendDisplayControlCMD(CMDPARM_DC_DISP_ON | CMDPARM_DC_CURSOR_OFF | CMDPARM_DC_BLINK_OFF);
    CLCD_DELAY();
    
    SendSetEntryModeCMD(CMDPARM_SEM_SHIFT_OFF | CMDPARM_SEM_CURSOR_RIGHT);
    CLCD_DELAY();
    
    SendClearDisplayCMD();
    CLCD_DELAY();
}


//------------------------------------------------------------------------------------------------

static void CLCD_SetShiftReg(uint8_t state)
{
    CLCD_SPI_CFG();
    CLCD_CS_PORT &= ~CLCD_CS;
    SPDR = state;
    while(!(SPSR & (1<<SPIF)))
        ;
    CLCD_CS_PORT |= CLCD_CS;
    state = SPDR;// clear SPIF flag
}


//------------------------------------------------------------------------------------------------
// Send a byte to the LCD controller. Whether it is a command or data depends on the setting of
// the LCD_CTRL_CMD_CHAR bit in CLCD_lcdState.
// Note that CLCD_SendByte() does not wait for the operation to finish on the LCD. Delay or use
// CLCD_WriteByte() when performing multiple operations in sequence.
static void CLCD_SendByte(uint8_t byte)
{
    uint8_t activeLCDs = CLCD_lcdState & CLCD_ALL_LCDS;
    // Set command, high nybble
    uint8_t portState = CLCD_lcdState;
    portState |= byte & 0xF0;
    CLCD_SetShiftReg(portState);
    
    // Set enable low, latch high nybble
    portState &= ~activeLCDs;
    CLCD_SetShiftReg(portState);
    
    // send low nybble.
    portState = ((byte << 4) & 0xF0) | CLCD_lcdState;
    CLCD_SetShiftReg(portState);
    
    // Set enable low, latch low nybble
    portState &= ~activeLCDs;
    CLCD_SetShiftReg(portState);
}


//------------------------------------------------------------------------------------------------

static void CLCD_SendCommand(uint8_t command)
{
    LCD_CTRL_SetCMD();
    CLCD_SendByte(command);
    LCD_CTRL_SetCHAR();
}


//------------------------------------------------------------------------------------------------

void CLCD_SelectLCDs(uint8_t lcds)
{
    CLCD_lcdState = (CLCD_lcdState & ~CLCD_ALL_LCDS) | lcds;
}


//------------------------------------------------------------------------------------------------

#if(CLCD_AUX_CONTROL)
void CLCD_SetAux(bool blState)
{
    if(blState) {
        CLCD_lcdState |= LCD_BL_CTL;
        CLCD_SetShiftReg(LCD_BL_CTL);
    }
    else {
        CLCD_lcdState &= ~LCD_BL_CTL;
        CLCD_SetShiftReg(0x00);
    }
}
#endif


//------------------------------------------------------------------------------------------------

void CLCD_WriteByte(uint8_t byte)
{
    CLCD_SendByte(byte);
    CLCD_DELAY();
}


//------------------------------------------------------------------------------------------------

void CLCD_SetCGRAM_Addr(uint8_t addr)
{
    CLCD_SendCommand(CMD_S_CGRAM_ADDR | (addr & 0x3F));
    CLCD_DELAY();
}


//------------------------------------------------------------------------------------------------

void CLCD_SetDDRAM_Addr(uint8_t addr)
{
    CLCD_SendCommand(CMD_S_DDRAM_ADDR | (addr & 0x7F));
    CLCD_DELAY();
}


//------------------------------------------------------------------------------------------------

void CLCD_LoadCustomChar(char charCode, const uint8_t * ch)
{
    CLCD_SetCGRAM_Addr(charCode << 3);
    for(uint8_t j = 0; j < 8; ++j)
        CLCD_WriteByte(*ch++);
}

void CLCD_LoadCustomChar_P(char charCode, const uint8_t * ch)
{
    CLCD_SetCGRAM_Addr(charCode << 3);
    for(uint8_t j = 0; j < 8; ++j)
        CLCD_WriteByte(pgm_read_byte(ch++));
}


//------------------------------------------------------------------------------------------------

void CLCD_MoveTo(uint8_t line, uint8_t x)
{
#if(CLCD_LINES == 1)
    CLCD_SetDDRAM_Addr(0x00 + x);
#endif // 1 line
    
#if(CLCD_LINES == 2)
    if(line == 0)
        CLCD_SetDDRAM_Addr(0x00 + x);
    else
        CLCD_SetDDRAM_Addr(0x40 + x);
#endif // 2 lines
    
#if(CLCD_LINES == 4)
#if(CLCD_LINE_WIDTH == 40)
    if(line < 2) {
        CLCD_SelectLCDs(CLCD_LCD0);
        if(line == 0)
            CLCD_SetDDRAM_Addr(0x00 + x);
        else if(line == 1)
            CLCD_SetDDRAM_Addr(0x40 + x);
    }
    else {
        CLCD_SelectLCDs(CLCD_LCD1);
        if(line == 2)
            CLCD_SetDDRAM_Addr(0x00 + x);
        else
            CLCD_SetDDRAM_Addr(0x40 + x);
    }
#else // is single-controller 4-line display
    if(line == 0)
        CLCD_SetDDRAM_Addr(0x00 + x);
    else if(line == 1)
        CLCD_SetDDRAM_Addr(0x40 + x);
    else if(line == 2)
        CLCD_SetDDRAM_Addr(0x14 + x);
    else
        CLCD_SetDDRAM_Addr(0x54 + x);
#endif // dual/single-controller 4-line display
#endif // 4 lines
}


//------------------------------------------------------------------------------------------------

void CLCD_WriteHexByte(uint8_t val)
{
    unsigned char highnybble = (val >> 4) & 0x0F;
    unsigned char lownybble = val & 0x0F;

    if(highnybble < 10)
        CLCD_SendByte('0' + highnybble);
    else
        CLCD_SendByte('A' + highnybble - 10);
    
    CLCD_DELAY();
    
    if(lownybble < 10)
        CLCD_SendByte('0' + lownybble);
    else
        CLCD_SendByte('A' + lownybble - 10);
}


//------------------------------------------------------------------------------------------------

void CLCD_WriteHexUInt8(uint8_t val)
{
    CLCD_SendByte('0');
    CLCD_DELAY();
    CLCD_SendByte('x');
    CLCD_DELAY();
    CLCD_WriteHexByte(val);
}


//------------------------------------------------------------------------------------------------

void CLCD_WriteHexUInt16(uint16_t val)
{
    CLCD_SendByte('0');
    CLCD_DELAY();
    CLCD_SendByte('x');
    CLCD_DELAY();
    CLCD_WriteHexByte((val >> 8) & 0xFF);
    CLCD_DELAY();
    CLCD_WriteHexByte(val & 0xFF);
}


//------------------------------------------------------------------------------------------------

void CLCD_WriteString(const char * str)
{
    for(; *str != '\0'; ++str)
        CLCD_WriteByte(*str);
}


//------------------------------------------------------------------------------------------------

void CLCD_WriteString_P(const prog_char * str)
{
    for(; pgm_read_byte(str) != '\0'; ++str)
        CLCD_WriteByte(pgm_read_byte(str));
}


//------------------------------------------------------------------------------------------------
#if(CLCD_ASYNC)
//------------------------------------------------------------------------------------------------

void CLCD_Update(char * bfr)
{
    // Zero the character index and start first line.
    // If an update is in progress, it will be restarted from the first character.
    CLCD_buffer = bfr;
    CLCD_updateState = CLCDSTATE_START_REFRESH;
}

bool CLCD_UpdateDone()
{
    return CLCD_updateState == CLCDSTATE_REFRESH_DONE;
}

void CLCD_UpdateStep()
{
    uint8_t line = CLCD_updateState >> 6;
    uint8_t charIdx = CLCD_updateState & 0x3F;
    
    if(charIdx == CLCDSTATE_REFRESH_DONE) {
        return;
    }
    else if(charIdx < CLCD_LINE_WIDTH) {
        CLCD_SendByte(*CLCD_buffer++);
//        CLCD_SendByte(CLCD_buffer[CLCD_updateState + line*CLCD_LINE_WIDTH]);
        ++CLCD_updateState;
    }
    else if(charIdx == CLCD_LINE_WIDTH) {
        if(line == 0) {
            // done with line 1, move to line 2 or set done
#if(CLCD_LINES == 1)
            CLCD_updateState = CLCDSTATE_REFRESH_DONE;
#else
            // second line is 0x40 on all display types
            CLCD_SendCommand(CMD_S_DDRAM_ADDR | 0x40);
            CLCD_updateState = (line + 1) << 6;
#endif
        }
#if(CLCD_LINES == 4)
#if(CLCD_LINE_WIDTH == 40)
        // 4x40 dual-controller displays: need to switch controllers
        else if(line == 1) {// done with line 2, move to line 3
            CLCD_SelectLCDs(CLCD_LCD1);
            CLCD_SendCommand(CMD_S_DDRAM_ADDR | 0x00);
            CLCD_updateState = (line + 1) << 6;
        }
        else if(line == 2) {// done with line 3, move to line 4
            CLCD_SendCommand(CMD_S_DDRAM_ADDR | 0x40);
            CLCD_updateState = (line + 1) << 6;
        }
#else // is single-controller 4-line display
        else if(line == 1) {// done with line 2, move to line 3
            CLCD_SendCommand(CMD_S_DDRAM_ADDR | 0x14);
            CLCD_updateState = (line + 1) << 6;
        }
        else if(line == 2) {// done with line 3, move to line 4
            CLCD_SendCommand(CMD_S_DDRAM_ADDR | 0x54);
            CLCD_updateState = (line + 1) << 6;
        }
#endif // dual/single-controller 4-line display
#endif // 4 lines
#if(CLCD_LINES > 1)
        else {// Done with last line
            CLCD_updateState = CLCDSTATE_REFRESH_DONE;
        }
#endif
    }
    else if(CLCD_updateState == CLCDSTATE_START_REFRESH) {
#if(CLCD_LINES == 4 && CLCD_LINE_WIDTH == 40)
        // 4x40 LCD, need to select first controller
        CLCD_SelectLCDs(CLCD_LCD0);
#endif
        CLCD_SendCommand(CMD_S_DDRAM_ADDR);// move to beginning of first line
        CLCD_updateState = 0x00;// first line, first character
    }
    else {
        CLCD_updateState = CLCDSTATE_REFRESH_DONE;
    }
}

//------------------------------------------------------------------------------------------------
#else // Asynchronous update is disabled
//------------------------------------------------------------------------------------------------

void CLCD_Update(char * bfr)
{
#if(CLCD_LINES == 1 || CLCD_LINES == 2)
    CLCD_SetDDRAM_Addr(0x00);// move to start of first line
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
#endif
#if(CLCD_LINES == 2)
    CLCD_SetDDRAM_Addr(0x40);// move to start of second line
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
#endif
    
#if(CLCD_LINES == 4)
#if(CLCD_LINE_WIDTH == 40)
    // 2-controller 4x40 display
    CLCD_SelectLCDs(CLCD_LCD0);
    CLCD_SetDDRAM_Addr(0x00);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SetDDRAM_Addr(0x40);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SelectLCDs(CLCD_LCD1);
    CLCD_SetDDRAM_Addr(0x00);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SetDDRAM_Addr(0x40);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
#else
    // Display four half-lines
    CLCD_SetDDRAM_Addr(0x00);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SetDDRAM_Addr(0x40);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SetDDRAM_Addr(0x14);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
    
    CLCD_SetDDRAM_Addr(0x54);
    for(uint8_t j = 0; j < CLCD_LINE_WIDTH; ++j) {
        CLCD_SendByte(*bfr++);
        CLCD_DELAY();
    }
#endif
#endif // 4-line displays
}
//------------------------------------------------------------------------------------------------
#endif // CLCD_ASYNC
//------------------------------------------------------------------------------------------------

//************************************************************************************************
