#include <LiquidCrystal.h>
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Arduino version compatibility Pre-Compiler Directives
#if defined(ARDUINO) && ARDUINO >= 100   // Arduino v1.0 and newer
#define I2C_WRITE Wire.write
#define I2C_READ Wire.read
#else                                   // Arduino Prior to v1.0 
#define I2C_WRITE Wire.send
#define I2C_READ Wire.receive
#endif
// Global Variables
int command = 0;       // This is the command char, in ascii form, sent from the serial port
int i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte test;
byte zero;
char  *Day[] = {"", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"};
char  *Mon[] = {"", "Jan", "Fev", "Mar", "Avr", "Mai", "Jun", "Jul", "Aou", "Sep", "Oct", "Nov", "Dec"};
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 3;

byte a = 0;
byte b = 0;
byte c = 0;
byte d = 0;
byte Col = 1;
byte t = 0;
byte cod = 0;
byte cou = 0;


// Convert normal decimal numbers to binary coded decimal
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

}


void setup() {
  Wire.begin();
  lcd.begin(16, 2);
  Serial.begin(57600);
  zero = 0x00;
}

void loop() {
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

  lcd.setCursor(0, 1);

  if (hour < 10)
    lcd.print("0");
  lcd.print(hour, DEC);
  lcd.print(":");
  if (minute < 10)
    lcd.print("0");
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second < 10)
    lcd.print("0");
  lcd.print(second, DEC);
  lcd.setCursor(0, 0);
  lcd.print(Day[dayOfWeek]);
  lcd.print(" ");
  lcd.print(dayOfMonth, DEC);
  lcd.print(" ");
  lcd.print(Mon[month]);
  lcd.print(" 20");
  if (year < 10)
    lcd.print("0");
  lcd.print(year, DEC);

a = hour / 10;
b = hour % 10;
c = minute / 10;
d = minute % 10;

Col = Col * 2;
if (Col > 8) {
  Col = 1;
} else {
}

if (Col == 1) {
t = a;
}

if (Col == 2) {
t = b;
}

if (Col == 4) {
t = c;
}

if (Col == 8) {
t = d;
}

if (t == 0) {
cod = 63;
}

if (t == 1) {
cou = 6;
}

if (t == 2) {
cou = 91;
}

if (t == 3) {
cou = 79;
}

if (t == 4) {
cou = 102;
}

if (t == 5) {
cou = 109;
}

if (t == 6) {
cou = 125;
}

if (t == 7) {
cou = 7;
}

if (t == 8) {
cou = 127;
}

if (t == 9) {
cou = 111;
}

delay (10);

digitalWrite(latchPin, LOW);
shiftOut(dataPin, clockPin, MSBFIRST, Col);
shiftOut(dataPin, clockPin, MSBFIRST, ~cou);
digitalWrite(latchPin, HIGH);

  delay(50);

}
//*****************************************************The End***********************
