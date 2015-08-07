
#include "Wire.h"
#include "Adafruit_BMP085.h"
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
// Global Variables
long w,h;
int command = 0;       // This is the command char, in ascii form, sent from the serial port     
int i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte psecond, pminute, phour, pdayOfWeek, pdayOfMonth, pmonth, pyear;
byte test;
byte zero;
long pe;
float temperature;
long ppe;
float ptemperature;

char  *Day[] = {
  "","Lundi","Mardi","Mercredi","Jeudi","Vendredi","Samedi","Dimanche"};
char  *Mon[] = {
  "","Jan","Fev","Mar","Avr","Mai","Jun","Jul","Aou","Sep","Oct","Nov","Dec"};

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers, Probably need to put in checks for valid numbers.

void setDateDs1307()                
{

  second = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); // Use of (byte) type casting and ascii math to achieve result.  
  minute = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  hour  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  dayOfWeek = (byte) (Serial.read() - 48);
  dayOfMonth = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  month = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  year= (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
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
  temperature = bmp.readTemperature();
  pe = bmp.readPressure();

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
  Serial.print(" ");
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

}
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A6 // defaut A4 mais bloque le I2C Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

long compt = 0;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
void setup(void) {
  Wire.begin();
  zero=0x00;
  Serial.begin(57600);
  bmp.begin();

  w = tft.width();
  h = tft.height();
  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 80);
  tft.print("Ecran de type :");
  tft.println(identifier);
  tft.println("Demarrage de l'application");
  delay(3000);
}


void loop(void) {
  getDateDs1307();

  temperature = bmp.readTemperature();
  pe = bmp.readPressure();


  tft.setCursor(10, 10);
  
    tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.println(Day[pdayOfWeek]);
  if (pdayOfMonth < 10)
    tft.print(" ");
  tft.print(pdayOfMonth);
  tft.print("/");
  if (pmonth < 10)
    tft.print("0");
  tft.print(pmonth);
  tft.print("/");
  if (pyear < 10)
    tft.print("0");
  tft.print("20");
  tft.println(pyear);
  if (phour < 10)
    tft.print("0");
  tft.print(phour);
  tft.print(":");
  if (pminute < 10)
    tft.print("0");
  tft.print(pminute);
  tft.print(":");
  if (psecond < 10)
    tft.print("0");
  tft.println(psecond);
  tft.print(ptemperature);
  tft.println("'C");
  tft.print(ppe);
  tft.println("PA");
  
  
  
  tft.setTextColor(RED);
  tft.setTextSize(4);
  tft.println(Day[dayOfWeek]);
  if (dayOfMonth < 10)
    tft.print(" ");
  tft.print(dayOfMonth);
  tft.print("/");
  if (month < 10)
    tft.print("0");
  tft.print(month);
  tft.print("/");
  if (year < 10)
    tft.print("0");
  tft.print("20");
  tft.println(year);
  if (hour < 10)
    tft.print("0");
  tft.print(hour);
  tft.print(":");
  if (minute < 10)
    tft.print("0");
  tft.print(minute);
  tft.print(":");
  if (second < 10)
    tft.print("0");
  tft.println(second);
  tft.print(temperature);
  tft.println("'C");
  tft.print(pe);
  tft.println("PA");




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
    Serial.print("Command: ");
    Serial.println(command);     // Echo command CHAR in ascii that was sent
  }
  command = 0;                 // reset command 
  delay(1000);
}

