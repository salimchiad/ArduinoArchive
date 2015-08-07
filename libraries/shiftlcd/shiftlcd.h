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
//
// Driver for 1-3 HD44780/KS0066U-compatible character LCDs through a 74595 shift register.
// Low resource usage is a priority: if asynchronous updates are disabled, only one byte of SRAM
// is used. The state machine for asynchronous updates uses only 3 more bytes. TODO: if cursor
// control is enabled, an additional byte is used. No timers are used.
//
// Up to 3 identical single-controller LCDs may be driven, or a 4x40 dual-controller
// display. Alternatively, a single 4x40 display or 2 smaller displays can be driven
// with the third enable line used for backlight control.
// 
// Non-identical LCDs also may be used, but CLCD_MoveTo() and CLCD_Update() will only
// work properly for one size. If a 4x40 display is used, these functions will automatically
// select its controllers. For a third display, the address must be set manually, and the LCD
// written to with CLCD_WriteByte() and CLCD_WriteString().
// 
// LCD_LINES may be 1, 2, or 4.
// 4 line displays with 20 characters or fewer per line are actually 2 line
// displays with each line split in half at 0x14.
// 4x40 displays are actually two 2x40 displays with separate controllers. Such displays
// may be configured as two separate LCDs instead.
// 
// The controller setup only matters for CLCD_MoveTo(), which automatically selects
// LCD 1 or LCD 2 for 4x40 displays, and CLCD_Update(), which updates both controllers
// on such displays. The rest of the API simply uses whatever controller or controllers
// are enabled.
// 
// DDRAM memory map:
// 1 line and 2 line displays:
// 0x00 to 0x27 for first line
// 0x40 to 0x67 for second line if it exists
// 
// 4x16, 4x20 displays:
// 0x00 to 0x13 for first line
// 0x40 to 0x53 for second line
// 0x14 to 0x27 for third line
// 0x54 to 0x67 for fourth line
// 
// 4x40 displays:
// 0x00 to 0x27 on controller 1 for first line
// 0x40 to 0x67 on controller 1 for second line
// 0x00 to 0x27 on controller 2 for third line
// 0x40 to 0x67 on controller 2 for fourth line
// 
// Custom characters:
// 5x7:
// Custom character codes start at 0x00 and go to 0x07, addresses are
// charcode << 3.
// Characters are 8 bytes: 7 lines and cursor position(?). Lowest 5 bits are displayed.
// 
// More information:
// http://www.electronic-engineering.ch/microchip/datasheets/lcd/lcd_data_sheets.html
// http://ouwehand.net/~peter/lcd/lcd0.shtml
// 
//************************************************************************************************

#ifndef SHIFTLCD_H
#define SHIFTLCD_H

#include <avr/pgmspace.h>
#include "common.h"


// Enable asynchronous updates to eliminate long LCD-refreshing delays,
// at the expense of code size, SRAM usage, and display speed.
#ifndef CLCD_ASYNC
#define CLCD_ASYNC 1
#endif

// If set, one shift register bit is used for LCD backlight control instead of a third controller
#ifndef CLCD_AUX_CONTROL
#define CLCD_AUX_CONTROL 1
#endif

#ifndef CLCD_LINE_WIDTH
//#define CLCD_LINE_WIDTH (16)
//#define CLCD_LINE_WIDTH (20)
#define CLCD_LINE_WIDTH (40)
#endif

#ifndef CLCD_LINES
//#define CLCD_LINES (2)
//#define CLCD_LINES (4)
#define CLCD_LINES (4)
#endif


#define CLCD_DELAY()     _delay_us(44)
#define CLCD_LONGDELAY() _delay_ms(2)

// SPI chip select configuration
//#define CLCD_SPI_CFG() SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0)
#define CLCD_SPI_CFG() do { \
    SPCR = (1<<SPE) | (1<<MSTR); \
    SPSR |= (1<<SPI2X); \
} while(0)

#define CLCD_CS_PORT PORTA
#define CLCD_CS_DDR DDRA
#define CLCD_CS (1 << PA0)


#define CLCD_CHARS (CLCD_LINE_WIDTH*CLCD_LINES)

// Pin config for the 74595 shift register
// Data bits are high 4 bits. Data is never read, so the read/write line is hard wired
// to ground. Data and register select may be shared by multiple modules, which leaves 3
// pins available for controller enable, backlight control, etc.
#define CLCD_LCD0 (0x04)
#define CLCD_LCD1 (0x02)

#if(CLCD_AUX_CONTROL)
#define LCD_BL_CTL (0x01)
#define CLCD_ALL_LCDS (CLCD_LCD0 | CLCD_LCD1)
#else
#define CLCD_ALL_LCDS (CLCD_LCD0 | CLCD_LCD1 | CLCD_LCD2)
#endif

#define LCD_CTRL_CMD_CHAR (0x08)


//------------------------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------------------------

// CLCD_Init():
// Initialize character LCD driver. SPI bus must be set up before this is called.
void CLCD_Init(void);

//------------------------------------------------------------------------------------------------
// CLCD_SelectLCDs():
// Select active LCDs. Multiple LCDs may be active simultaneously, which can be useful for
// loading custom characters or duplicating output.
void CLCD_SelectLCDs(uint8_t lcds);


#if(CLCD_AUX_CONTROL)
//------------------------------------------------------------------------------------------------
// CLCD_SetAux(): Set auxiliary output state (typically backlight/LCD power)
// Uses output that would otherwise be used for third controller.
void CLCD_SetAux(bool blState);
#endif


//------------------------------------------------------------------------------------------------
// CLCD_SetDDRAM_Addr(uint8_t addr), CLCD_SetCGRAM_Addr(uint8_t addr):
// Change current CGRAM or DDRAM address
void CLCD_SetDDRAM_Addr(uint8_t addr);
void CLCD_SetCGRAM_Addr(uint8_t addr);


//------------------------------------------------------------------------------------------------
// CLCD_LoadCustomChar(char charCode, const uint8_t * ch):
//   Loads custom character for the selected LCD(s). For 5x7 fonts, 8 character codes are
// available, starting at 0x00. Characters are 8 bytes 
// MSB           LSB
//   x x x # # # # # TOP
//   x x x # # # # #
//   x x x # # # # #
//   x x x # # # # #
//   x x x # # # # #
//   x x x # # # # #
//   x x x # # # # # BOTTOM
//   x x x # # # # # last line is OR'd with cursor
// 
// Note: current cursor position is lost and the LCD left in CGRAM write mode. CLCD_MoveTo()
// or CLCD_SetDDRAM_Addr() must be called before the contents of the display can be changed.
// If using 4x40 LCDs, both CLCD_LCD0 and CLCD_LCD1 should be manually selected before this
// call. (unless the top and bottom controllers are to receive different custom characters)
void CLCD_LoadCustomChar(char charCode, const uint8_t * ch);
void CLCD_LoadCustomChar_P(char charCode, const uint8_t * ch);


//------------------------------------------------------------------------------------------------
// CLCD_MoveTo(uint8_t line, uint8_t col):
//   Set DDRAM address for specified character position. Line and column are both zero based.
//   For dual-controller LCDs, also select appropriate controller.
void CLCD_MoveTo(uint8_t line, uint8_t col);


//------------------------------------------------------------------------------------------------
// CLCD_WriteChar(uint8_t byte), CLCD_WriteByte(uint8_t byte):
//   Write to to CGRAM or DDRAM. These functions are synonyms: CLCD_WriteByte() is intended for
// binary data and CLCD_WriteChar() for characters to display, but either can be used in place of
// the other.
#define CLCD_WriteChar CLCD_WriteByte
void CLCD_WriteByte(uint8_t byte);


//------------------------------------------------------------------------------------------------
// CLCD_WriteString(), CLCD_WriteString_P():
// Write null-delimited string. String must fit on line, will not automatically wrap to next line.
void CLCD_WriteString(const char * character);
void CLCD_WriteString_P(const prog_char * character);


//------------------------------------------------------------------------------------------------
// CLCD_Update():
//   Redraw LCD from buffer. Buffer must have room for at least as many characters as are
// available on the display. Multiple lines are simply concatenated in the buffer.
// Contents of the buffer should not be modified until CLCD_UpdateDone() returns true.
// Typical run time <50 us per character, full display refresh (when not using asynchronous
// updating):
//     16 characters: 0.8 ms
//     2x16 characters: 1.6 ms
//     2x20 characters: 2 ms
//     2x40, 4x20 characters: 4 ms
//     4x40 characters: 8 ms, 125 Hz
// 
// These numbers will vary depending on SPI speed.
void CLCD_Update(char * bfr);


//------------------------------------------------------------------------------------------------
//	CLCD_UpdateStep():
//   If asynchronous updates are enabled, CLCD_UpdateStep() must be called no more than once
// every 40 microseconds. Most calls will execute in the time required for 2 SPI transfers,
// ~2 us at 16 MHz at default settings...CLCD_UpdateStep() should not be called from an
// interrupt.
// CLCD_UpdateDone(): Returns true if display update is complete.
// 
//   If asynchronous updates are disabled, these functions are no-ops. CLCD_UpdateDone() will
// always return true in this case.
#if(CLCD_ASYNC)
void CLCD_UpdateStep(void);
bool CLCD_UpdateDone(void);
#else
#define CLCD_UpdateStep()
#define CLCD_UpdateDone() (true)
#endif

//************************************************************************************************
#endif // SHIFTLCD_H
