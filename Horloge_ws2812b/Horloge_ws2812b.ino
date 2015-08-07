#include <Adafruit_NeoPixel.h>
#include "LedControl.h"
#define PIN 45
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
#include "Adafruit_BMP085.h"
int temperature;
long pe;
Adafruit_BMP085 bmp;

LedControl lc = LedControl(13, 12, 11, 1);
// Global Variables
char command = 0;       // This is the command char, in ascii form, sent from the serial port     
byte i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year, Heure, Minute, Heure1, Heure2;
byte test, t, r;
byte R, G, B;
byte zero;
byte Z1, Z2, Z3, Y1, Y2 ,Y3, X1, X2, X3, W1, W2, W3, V1, V2, V3;
unsigned int Z;
char  *Day[] = {
  "", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"};
char  *Mon[] = {
  "","Jan","Fev","Mar","Avr","Mai","Jun","Jul","Aou","Sep","Oct","Nov","Dec"};
byte nombre, puissance;
byte a,b,c,d,e,f,g,h;
unsigned char code[25]= {
  126,48,109,121,51,91,95,112,127,123,254,176,237,249,179,219,223,240,255,251,0,99,78,103,119};

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
  delay(10);
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

}
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  bmp.begin();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Wire.begin();
  Serial.begin(115200); 
  zero=0x00;
  Z1 = 10;
  Z2 = 10;
  Z3 = 10;
  Y1 = 0;
  Y2 = 20;
  Y3 = 30;
  X1 = 255;
  X2 = 0;
  X3 = 0;
  W1 = 0;
  W2 = 255;
  W3 = 0;
  V1 = 0;
  V2 = 0;
  V3 = 255;
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 15);
  /* and clear the display */
  lc.clearDisplay(0);


}


void loop()
{

  if (Serial.available()) {      // Look for char in serial que and process if found
    command = Serial.read();
    if (command == 84 || command == 116) {      //If command = "Tt" Set Date
      setDateDs1307();
      getDateDs1307();
      Serial.println(" ");
    }
    else if (command == 76 || command == 108) {      //If command = "Ll" Read Date ... BBR
      getDateDs1307();
      Serial.println(" ");
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
    else if (command == 81 || command == 113) {      //If command = "Qq" RTC1307 Memory Functions
      delay(100);
      if (Serial.available()) {
        command = Serial.read();
        if (command == 49) {        //If command = "1" RTC1307 Initialize Memory - All Data will be set to
          // 255 (0xff).  Therefore 255 or 0 will be an invalid value.
          Wire.beginTransmission(DS1307_I2C_ADDRESS);   // 255 will be the init value and 0 will be considered
          // an error that occurs when the RTC is in Battery mode.
          I2C_WRITE(0x08); // Set the register pointer to be just past the date/time registers.
          for (byte i = 1; i <= 24; i++) {
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
    else if (command == 82 || command == 114){   // commande R ou r
      R = Serial.parseInt();
      Serial.println(R);
    }

    else if (command == 71 || command == 103){   // commande G ou g
      G = Serial.parseInt();
      Serial.println(G);
    }

    else if (command == 66 || command == 98){   // commande B ou b
      B = Serial.parseInt();
      Serial.println(B);
    }

    else if (command == 122 || command == 90){   // commande Z ou 
      Z1 = R;
      Z2 = G;
      Z3 = B;
    }

    else if (command == 121 || command == 89){   // commande Y ou y
      Y1 = R;
      Y2 = G;
      Y3 = B;
    }

    else if (command == 120 || command == 88){   // commande X ou x
      X1 = R;
      X2 = G;
      X3 = B;
    }

    else if (command == 119 || command == 87){   // commande W ou w
      W1 = R;
      W2 = G;
      W3 = B;
    }

    else if (command == 118 || command == 86){   // commande V ou v
      V1 = R;
      V2 = G;
      V3 = B;
    }

    else if (command == 117 || command == 85){   // commande U ou u
      Z = Serial.parseInt();
      Serial.println(Z);
    }



    Serial.print("Command: ");
    Serial.println(command);     // Echo command CHAR in ascii that was sent
  }
  command = 0;                 // reset command

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

  if(hour >= 12) {
    Heure = (hour - 12.0) * 5;
  } 
  else {
    Heure = hour * 5;
  }

  Heure = Heure + (minute / 12);
  Minute = minute + 1;
  Heure1 = Heure + 1;
  Heure2 = Heure - 1;

  if (Heure == 0){
    Heure2 = 59;
  }

  if (Heure == 59){
    Heure1 = 0;
  }

  if ( Minute == 60){
    Minute = 0;
  }

  if(Z == 0){

    for(i = 0; i < 60; i++){ //definition couleur du fond
      strip.setPixelColor(i, Z1, Z2, Z3);
    }

    for (i = 0; i < 60; i=i+5){  //definition de la couleur des reperes de 5 secondes
      strip.setPixelColor(i, Y1, Y2, Y3);
    }
  }
  else if (Z/10000==1) {

    for(i = 0; i < 60; i++){ //definition couleur du fond
      strip.setPixelColor(i, Z1, Z2, Z3);

      nombre = (Z/100)%100;
      puissance = (Z%100)*2.5;

      for(i = 0; i < nombre; i++){ //definition couleur du fond
        for (i = 0; i < nombre; i++){  
          strip.setPixelColor(i, puissance, puissance, puissance);
        }

      }
    }
  }


  t = second;
  r = 60 - t;

  strip.setPixelColor(Heure, X1, X2, X3);
  strip.setPixelColor(Heure1, (X1/5), (X2/5), (X3/5));
  strip.setPixelColor(Heure2, (X1/5), (X2/5), (X3/5));
  strip.setPixelColor(minute, ((W1 * r) / 60), ((W2 * r) / 60), ((W3 * r) / 60));
  strip.setPixelColor(Minute, ((W1 * t) / 60), ((W2 * t) / 60), ((W3 * t) / 60));
  strip.setPixelColor(second, V1, V2, V3);
  strip.show();

  if(second < 16){
    a = 20;
    b = hour / 10;
    c = ((hour % 10) + 10);
    d = minute / 10;
    e = ((minute % 10) + 10);
    f = second / 10;
    g = second % 10;
    h = 20;
  }

  if(second >= 16 && second < 31){
    a = dayOfMonth / 10;
    b = ((dayOfMonth % 10) + 10);
    c = month / 10;
    d = ((month % 10) + 10);
    e = 2;
    f = 0;
    g = year / 10;
    h = year % 10;
  }

  if(second >= 31 && second < 45){
    temperature = bmp.readTemperature()*100;
    a = 20;
    b = temperature / 1000;
    c = (((temperature / 100) % 10) + 10) ;
    d = (temperature / 10) % 10;
    e = temperature % 10;
    f = 21;
    g = 22;
    h = 20;
  }

  if(second >= 46 && second < 60){
    pe = bmp.readPressure();
    a = pe / 100000;
    b = (pe / 10000) % 10;
    c = (pe / 1000) % 10;
    d = (pe / 100) % 10;
    e = (pe / 10) % 10;
    f = pe % 10;
    g = 23;
    h = 24;
  }





  lc.setRow(0, 0, code[a]);
  lc.setRow(0, 1, code[b]);
  lc.setRow(0, 2, code[c]);
  lc.setRow(0, 3, code[d]);
  lc.setRow(0, 4, code[e]);
  lc.setRow(0, 5, code[f]);
  lc.setRow(0, 6, code[g]);
  lc.setRow(0, 7, code[h]);

  delay(100);


}

