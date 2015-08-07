// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
#include "Wire.h"
#include <math.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
byte test;
int i;
int command = 0;       // This is the command char, in ascii form, sent from the serial port   

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Arduino version compatibility Pre-Compiler Directives
#if defined(ARDUINO) && ARDUINO >= 100   // Arduino v1.0 and newer
#define I2C_WRITE Wire.write
#define I2C_READ Wire.read
#else                                   // Arduino Prior to v1.0 
#define I2C_WRITE Wire.send
#define I2C_READ Wire.receive
#endif

byte zero;

char  *Day[] = {
  "", "Lu", "Ma", "Me", "Je", "Ve", "Sa", "Di"
};
char  *Mon[] = {
  "", "Jan", "Fev", "Mar", "Avr", "Mai", "Jun", "Jul", "Aou", "Sep", "Oct", "Nov", "Dec"
};

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A6 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

float second, minute, hour, pSec, pMin, pHour;
byte dayOfWeek, dayOfMonth, month, year;
unsigned int x, y, w, h;
long color;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); 
  Serial.print(tft.width()); 
  Serial.print("x"); 
  Serial.println(tft.height());
  w = tft.width();
  h = tft.height();

  tft.reset();

  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } 
  else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } 
  else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } 
  else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } 
  else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } 
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
}



void loop(void) {
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
  else {
    pSec = second;
    pMin = minute;
    pHour = hour;

    getDateDs1307();

    minute = (minute + (second / 60));
    hour = (hour + (minute / 60));

    tft.drawCircle(w/2, h/2, w/2.01, WHITE);

    tft.drawLine(w/2, h/2, (w/2)+((w/2)*sin(pSec*0.104717)),(h/2)+((w/2)*-cos(pSec*0.104717)), BLACK);
    tft.drawLine(w/2, h/2, (w/2)+((w/2.5)*sin(pMin*0.104717)),(h/2)+((w/2.5)*-cos(pMin*0.104717)), BLACK);
    tft.drawLine(w/2, h/2, (w/2)+((w/4)*sin(pHour*0.52359878)),(h/2)+((w/4)*-cos(pHour*0.52359878)), BLACK);

    for(int i=0; i<60; i++){
      tft.drawLine((w/2)+((w/2.5)*sin(i*0.104717)), (h/2)+((w/2.5)*-cos(i*0.104717)), (w/2)+((w/2)*sin(i*0.104717)),(h/2)+((w/2)*-cos(i*0.104717)), CYAN);
    }

    for(int i=0; i<12; i++){
      tft.drawLine((w/2)+((w/3)*sin(i*0.52359878)), (h/2)+((w/3)*-cos(i*0.52359878)), (w/2)+((w/2)*sin(i*0.52359878)),(h/2)+((w/2)*-cos(i*0.52359878)), YELLOW);
    }

    tft.drawLine(w/2, h/2, (w/2)+((w/2)*sin(second*0.104717)),(h/2)+((w/2)*-cos(second*0.104717)), GREEN);
    tft.drawLine(w/2, h/2, (w/2)+((w/2.5)*sin(minute*0.104717)),(h/2)+((w/2.5)*-cos(minute*0.104717)), BLUE);
    tft.drawLine(w/2, h/2, (w/2)+((w/4)*sin(hour*0.52359878)),(h/2)+((w/4)*-cos(hour*0.52359878)), RED);

    delay(1000);
  }

}





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
void getDateDs1307() {
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




}










