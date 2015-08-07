/*
Scrolling text example code
 Modified from: http://www.arduino.cc/playground/Code/PCD8544
 */
#include "Wire.h"
#include "Adafruit_BMP085.h"
#include "stdio.h"
// The pins to use on the arduino
#define PIN_SCE   7
#define PIN_RESET 6
#define PIN_DC    5
#define PIN_SDIN  4
#define PIN_SCLK  3
Adafruit_BMP085 bmp;

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Arduino version compatibility Pre-Compiler Directives
#if defined(ARDUINO) && ARDUINO >= 100   // Arduino v1.0 and newer
#define I2C_WRITE Wire.write
#define I2C_READ Wire.read
#else                                   // Arduino Prior to v1.0 
#define I2C_WRITE Wire.send
#define I2C_READ Wire.receive
#endif

int command = 0;       // This is the command char, in ascii form, sent from the serial port
int i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte test;
byte zero;
char  *Day[] = {
  "", "Lu", "Ma", "Me", "Je", "Ve", "Sa", "Di"};
char  *Mon[] = {
  "", "Jan", "Fev", "Mar", "Avr", "Mai", "Jun", "Jul", "Aou", "Sep", "Oct", "Nov", "Dec"};
char Date[13];
char Heure[13];
char Press[13];
long pe;
char pef[7];
char Temp[13];
char hourf[3];
char minutef[3];
char secondf[3];
char jourf[3];
char tempef[3];
char tempedf[3];
char tempa[13];
int temperature, tempf, tempdf;

// COnfiguration for the LCD
#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_CMD   0

// Size of the LCD
#define LCD_X     84
#define LCD_Y     48

int scrollPosition = -10;

static const byte ASCII[][5] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00        } // 20
  , {
    0x00, 0x00, 0x5f, 0x00, 0x00        } // 21 !
  , {
    0x00, 0x07, 0x00, 0x07, 0x00        } // 22 "
  , {
    0x14, 0x7f, 0x14, 0x7f, 0x14        } // 23 #
  , {
    0x24, 0x2a, 0x7f, 0x2a, 0x12        } // 24 $
  , {
    0x23, 0x13, 0x08, 0x64, 0x62        } // 25 %
  , {
    0x36, 0x49, 0x55, 0x22, 0x50        } // 26 &
  , {
    0x00, 0x05, 0x03, 0x00, 0x00        } // 27 '
  , {
    0x00, 0x1c, 0x22, 0x41, 0x00        } // 28 (
  , {
    0x00, 0x41, 0x22, 0x1c, 0x00        } // 29 )
  , {
    0x14, 0x08, 0x3e, 0x08, 0x14        } // 2a *
  , {
    0x08, 0x08, 0x3e, 0x08, 0x08        } // 2b +
  , {
    0x00, 0x50, 0x30, 0x00, 0x00        } // 2c ,
  , {
    0x08, 0x08, 0x08, 0x08, 0x08        } // 2d -
  , {
    0x00, 0x60, 0x60, 0x00, 0x00        } // 2e .
  , {
    0x20, 0x10, 0x08, 0x04, 0x02        } // 2f /
  , {
    0x3e, 0x51, 0x49, 0x45, 0x3e        } // 30 0
  , {
    0x00, 0x42, 0x7f, 0x40, 0x00        } // 31 1
  , {
    0x42, 0x61, 0x51, 0x49, 0x46        } // 32 2
  , {
    0x21, 0x41, 0x45, 0x4b, 0x31        } // 33 3
  , {
    0x18, 0x14, 0x12, 0x7f, 0x10        } // 34 4
  , {
    0x27, 0x45, 0x45, 0x45, 0x39        } // 35 5
  , {
    0x3c, 0x4a, 0x49, 0x49, 0x30        } // 36 6
  , {
    0x01, 0x71, 0x09, 0x05, 0x03        } // 37 7
  , {
    0x36, 0x49, 0x49, 0x49, 0x36        } // 38 8
  , {
    0x06, 0x49, 0x49, 0x29, 0x1e        } // 39 9
  , {
    0x00, 0x36, 0x36, 0x00, 0x00        } // 3a :
  , {
    0x00, 0x56, 0x36, 0x00, 0x00        } // 3b ;
  , {
    0x08, 0x14, 0x22, 0x41, 0x00        } // 3c <
  , {
    0x14, 0x14, 0x14, 0x14, 0x14        } // 3d =
  , {
    0x00, 0x41, 0x22, 0x14, 0x08        } // 3e >
  , {
    0x02, 0x01, 0x51, 0x09, 0x06        } // 3f ?
  , {
    0x32, 0x49, 0x79, 0x41, 0x3e        } // 40 @
  , {
    0x7e, 0x11, 0x11, 0x11, 0x7e        } // 41 A
  , {
    0x7f, 0x49, 0x49, 0x49, 0x36        } // 42 B
  , {
    0x3e, 0x41, 0x41, 0x41, 0x22        } // 43 C
  , {
    0x7f, 0x41, 0x41, 0x22, 0x1c        } // 44 D
  , {
    0x7f, 0x49, 0x49, 0x49, 0x41        } // 45 E
  , {
    0x7f, 0x09, 0x09, 0x09, 0x01        } // 46 F
  , {
    0x3e, 0x41, 0x49, 0x49, 0x7a        } // 47 G
  , {
    0x7f, 0x08, 0x08, 0x08, 0x7f        } // 48 H
  , {
    0x00, 0x41, 0x7f, 0x41, 0x00        } // 49 I
  , {
    0x20, 0x40, 0x41, 0x3f, 0x01        } // 4a J
  , {
    0x7f, 0x08, 0x14, 0x22, 0x41        } // 4b K
  , {
    0x7f, 0x40, 0x40, 0x40, 0x40        } // 4c L
  , {
    0x7f, 0x02, 0x0c, 0x02, 0x7f        } // 4d M
  , {
    0x7f, 0x04, 0x08, 0x10, 0x7f        } // 4e N
  , {
    0x3e, 0x41, 0x41, 0x41, 0x3e        } // 4f O
  , {
    0x7f, 0x09, 0x09, 0x09, 0x06        } // 50 P
  , {
    0x3e, 0x41, 0x51, 0x21, 0x5e        } // 51 Q
  , {
    0x7f, 0x09, 0x19, 0x29, 0x46        } // 52 R
  , {
    0x46, 0x49, 0x49, 0x49, 0x31        } // 53 S
  , {
    0x01, 0x01, 0x7f, 0x01, 0x01        } // 54 T
  , {
    0x3f, 0x40, 0x40, 0x40, 0x3f        } // 55 U
  , {
    0x1f, 0x20, 0x40, 0x20, 0x1f        } // 56 V
  , {
    0x3f, 0x40, 0x38, 0x40, 0x3f        } // 57 W
  , {
    0x63, 0x14, 0x08, 0x14, 0x63        } // 58 X
  , {
    0x07, 0x08, 0x70, 0x08, 0x07        } // 59 Y
  , {
    0x61, 0x51, 0x49, 0x45, 0x43        } // 5a Z
  , {
    0x00, 0x7f, 0x41, 0x41, 0x00        } // 5b [
  , {
    0x02, 0x04, 0x08, 0x10, 0x20        } // 5c ¥
  , {
    0x00, 0x41, 0x41, 0x7f, 0x00        } // 5d ]
  , {
    0x04, 0x02, 0x01, 0x02, 0x04        } // 5e ^
  , {
    0x40, 0x40, 0x40, 0x40, 0x40        } // 5f _
  , {
    0x00, 0x01, 0x02, 0x04, 0x00        } // 60 `
  , {
    0x20, 0x54, 0x54, 0x54, 0x78        } // 61 a
  , {
    0x7f, 0x48, 0x44, 0x44, 0x38        } // 62 b
  , {
    0x38, 0x44, 0x44, 0x44, 0x20        } // 63 c
  , {
    0x38, 0x44, 0x44, 0x48, 0x7f        } // 64 d
  , {
    0x38, 0x54, 0x54, 0x54, 0x18        } // 65 e
  , {
    0x08, 0x7e, 0x09, 0x01, 0x02        } // 66 f
  , {
    0x0c, 0x52, 0x52, 0x52, 0x3e        } // 67 g
  , {
    0x7f, 0x08, 0x04, 0x04, 0x78        } // 68 h
  , {
    0x00, 0x44, 0x7d, 0x40, 0x00        } // 69 i
  , {
    0x20, 0x40, 0x44, 0x3d, 0x00        } // 6a j
  , {
    0x7f, 0x10, 0x28, 0x44, 0x00        } // 6b k
  , {
    0x00, 0x41, 0x7f, 0x40, 0x00        } // 6c l
  , {
    0x7c, 0x04, 0x18, 0x04, 0x78        } // 6d m
  , {
    0x7c, 0x08, 0x04, 0x04, 0x78        } // 6e n
  , {
    0x38, 0x44, 0x44, 0x44, 0x38        } // 6f o
  , {
    0x7c, 0x14, 0x14, 0x14, 0x08        } // 70 p
  , {
    0x08, 0x14, 0x14, 0x18, 0x7c        } // 71 q
  , {
    0x7c, 0x08, 0x04, 0x04, 0x08        } // 72 r
  , {
    0x48, 0x54, 0x54, 0x54, 0x20        } // 73 s
  , {
    0x04, 0x3f, 0x44, 0x40, 0x20        } // 74 t
  , {
    0x3c, 0x40, 0x40, 0x20, 0x7c        } // 75 u
  , {
    0x1c, 0x20, 0x40, 0x20, 0x1c        } // 76 v
  , {
    0x3c, 0x40, 0x30, 0x40, 0x3c        } // 77 w
  , {
    0x44, 0x28, 0x10, 0x28, 0x44        } // 78 x
  , {
    0x0c, 0x50, 0x50, 0x50, 0x3c        } // 79 y
  , {
    0x44, 0x64, 0x54, 0x4c, 0x44        } // 7a z
  , {
    0x00, 0x08, 0x36, 0x41, 0x00        } // 7b {
  , {
    0x00, 0x00, 0x7f, 0x00, 0x00        } // 7c |
  , {
    0x00, 0x41, 0x36, 0x08, 0x00        } // 7d }
  , {
    0x10, 0x08, 0x08, 0x10, 0x08        } // 7e ←
  , {
    0x00, 0x06, 0x09, 0x09, 0x06        } // 7f →
};

byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

void setDateDs1307()
{

  second = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); // Use of (byte) type casting and ascii math to achieve result.
  minute = (byte) ((Serial.read() - 48) * 10 +  (Serial.read() - 48));
  hour  = (byte) ((Serial.read() - 48) * 10 +  (Serial.read() - 48));
  dayOfWeek = (byte) (Serial.read() - 48);
  dayOfMonth = (byte) ((Serial.read() - 48) * 10 +  (Serial.read() - 48));
  month = (byte) ((Serial.read() - 48) * 10 +  (Serial.read() - 48));
  year = (byte) ((Serial.read() - 48) * 10 +  (Serial.read() - 48));
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  I2C_WRITE(decToBcd(second) & 0x7f);    // 0 to bit 7 starts the clock
  I2C_WRITE(decToBcd(minute));
  I2C_WRITE(decToBcd(hour));      // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  I2C_WRITE(decToBcd(dayOfWeek));
  I2C_WRITE(decToBcd(dayOfMonth));
  I2C_WRITE(decToBcd(month));
  I2C_WRITE(decToBcd(year));
  Wire.endTransmission();
}

// Gets the date and time from the ds1307 and prints result
void getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(I2C_READ() & 0x7f);
  minute     = bcdToDec(I2C_READ());
  hour       = bcdToDec(I2C_READ() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(I2C_READ());
  dayOfMonth = bcdToDec(I2C_READ());
  month      = bcdToDec(I2C_READ());
  year       = bcdToDec(I2C_READ());

  if (hour < 10)
    Serial.print("0");
  Serial.print(hour, DEC);
  Serial.print(":");
  if (minute < 10)
    Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
    Serial.print("0");
  Serial.print(second, DEC);
  Serial.print("  ");
  Serial.print(Day[dayOfWeek]);
  Serial.print(", ");
  Serial.print(dayOfMonth, DEC);
  Serial.print(" ");
  Serial.print(Mon[month]);
  Serial.print(" 20");
  if (year < 10)
    Serial.print("0");
  Serial.println(year, DEC);
  //Ajout de la pression et temperature
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.println();
  delay(5);


}


void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE,   OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC,    OUTPUT);
  pinMode(PIN_SDIN,  OUTPUT);
  pinMode(PIN_SCLK,  OUTPUT);

  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LcdWrite(LCD_CMD, 0x21);  // LCD Extended Commands.
  LcdWrite(LCD_CMD, 0xB1);  // Set LCD Vop (Contrast). //B1
  LcdWrite(LCD_CMD, 0x04);  // Set Temp coefficent. //0x04
  LcdWrite(LCD_CMD, 0x13);  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_CMD, 0x0d);  // LCD in normal mode. 0x0d for inverse
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

/**
 * gotoXY routine to position cursor
 * x - range: 0 to 84
 * y - range: 0 to 5
 */
void gotoXY(int x, int y)
{
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.
}


void setup(void)
{
  Wire.begin();
  Serial.begin(57600);
  bmp.begin();
  zero = 0x00;

  LcdInitialise();
  LcdClear();
}

void loop(void)
{

  if (Serial.available()) {      // Look for char in serial que and process if found
    command = Serial.read();
    if (command == 84 || command == 116) {      //If command = "Tt" Set Date
      setDateDs1307();
      getDateDs1307();
      Serial.println(" ");
    }
    else if (command == 82 || command == 114) {      //If command = "Rr" Read Date ... BBR
      getDateDs1307();
      Serial.println(" ");
    }
    else if (command == 81 || command == 113) {      //If command = "Qq" RTC1307 Memory Functions
      delay(100);
      if (Serial.available()) {
        command = Serial.read();
        if (command == 49) {        //If command = "1" RTC1307 Initialize Memory - All Data will be set to
          // 255 (0xff).  Therefore 255 or 0 will be an invalid value.
          Wire.beginTransmission(DS1307_I2C_ADDRESS);   // 255 will be the init value and 0 will be considered
          // an error that occurs when the RTC is in Battery mode.
          I2C_WRITE(0x08); // Set the register pointer to be just past the date/time registers.
          for (i = 1; i <= 24; i++) {
            I2C_WRITE(0Xff);
            delay(10);
          }
          Wire.endTransmission();
          Wire.beginTransmission(DS1307_I2C_ADDRESS);
          I2C_WRITE(0x21); // Set the register pointer to 33 for second half of registers. Only 32 writes per connection allowed.
          for (i = 1; i <= 33; i++) {
            I2C_WRITE(0Xff);
            delay(10);
          }
          Wire.endTransmission();
          getDateDs1307();
          Serial.println(": RTC1307 Initialized Memory");
        }
        else if (command == 50) {      //If command = "2" RTC1307 Memory Dump
          getDateDs1307();
          Serial.println(": RTC 1307 Dump Begin");
          Wire.beginTransmission(DS1307_I2C_ADDRESS);
          I2C_WRITE(zero);
          Wire.endTransmission();
          Wire.requestFrom(DS1307_I2C_ADDRESS, 32);
          for (i = 0; i <= 31; i++) {  //Register 0-31 - only 32 registers allowed per I2C connection
            test = I2C_READ();
            Serial.print(i);
            Serial.print(": ");
            Serial.print(test, DEC);
            Serial.print(" : ");
            Serial.println(test, HEX);
          }
          Wire.beginTransmission(DS1307_I2C_ADDRESS);
          I2C_WRITE(0x20);
          Wire.endTransmission();
          Wire.requestFrom(DS1307_I2C_ADDRESS, 32);
          for (i = 32; i <= 63; i++) {         //Register 32-63 - only 32 registers allowed per I2C connection
            test = I2C_READ();
            Serial.print(i);
            Serial.print(": ");
            Serial.print(test, DEC);
            Serial.print(" : ");
            Serial.println(test, HEX);
          }
          Serial.println(" RTC1307 Dump end");
        }
      }
    }
    Serial.print("Command: ");
    Serial.println(command);     // Echo command CHAR in ascii that was sent
  }
  command = 0;                 // reset command

  delay(50);

  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  second     = bcdToDec(I2C_READ() & 0x7f);
  minute     = bcdToDec(I2C_READ());
  hour       = bcdToDec(I2C_READ() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(I2C_READ());
  dayOfMonth = bcdToDec(I2C_READ());
  month      = bcdToDec(I2C_READ());
  year       = bcdToDec(I2C_READ());

  if (second % 10 == 0 || second % 10 == 6) {
  }

  delay(10);

  if (hour < 10){
    sprintf (hourf, " %d", hour);
  } 
  else {
    sprintf (hourf, "%d", hour);
  }

  if (minute < 10){
    sprintf (minutef, "0%d", minute);
  } 
  else {
    sprintf (minutef, "%d", minute);
  }

  if (second < 10){
    sprintf (secondf, "0%d", second);
  } 
  else {
    sprintf (secondf, "%d", second);
  }

  if (dayOfMonth < 10){
    sprintf (jourf, "0%d", dayOfMonth);
  } 
  else {
    sprintf (jourf, "%d", dayOfMonth);
  }

  if (dayOfMonth < 10){
    sprintf (jourf, "0%d", dayOfMonth);
  } 
  else {
    sprintf (jourf, "%d", dayOfMonth);
  }

  temperature = bmp.readTemperature() * 100;
  tempf = temperature / 100;
  tempdf = temperature % 100;
  if (tempf < 10){
    sprintf (tempef, "0%d", tempf);
  } 
  else {
    sprintf (tempef, "%d", tempf);
  }
  if (tempdf < 10){
    sprintf (tempedf, "0%d", tempdf);
  } 
  else {
    sprintf (tempedf, "%d", tempdf);
  }

  pe = bmp.readPressure();
  if (pe < 100000){
    sprintf (pef, "0%ld", pe);
  } 
  else {
    sprintf (pef, "%ld", pe);
  }



  sprintf (Heure, "%s:%s:%s", hourf, minutef, secondf);
  sprintf (Date, "%s %s %s %d", Day[dayOfWeek], jourf, Mon[month], year);
  sprintf (tempa, "Temp %s,%s'C", tempef, tempedf);
  sprintf (Press, "Pe %s Pa", pef);

  gotoXY(7, 0);
  LcdString("Reveil mat'");
  gotoXY(0, 1);
  LcdString(Date);
  gotoXY(12, 2);
  LcdString(Heure);
  gotoXY(0, 3);
  LcdString(tempa);
  gotoXY(0, 4);
  LcdString(Press);
  delay(10);
}




