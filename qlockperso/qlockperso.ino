#include "Wire.h"
#include "LedControl.h"
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Arduino version compatibility Pre-Compiler Directives
#if defined(ARDUINO) && ARDUINO >= 100   // Arduino v1.0 and newer
#define I2C_WRITE Wire.write 
#define I2C_READ Wire.read
#else                                   // Arduino Prior to v1.0 
#define I2C_WRITE Wire.send 
#define I2C_READ Wire.receive
#endif 
/*Port Definitions*/
LedControl lc = LedControl(12,11,10,1);
// Global Variables
int command = 0;       // This is the command char, in ascii form, sent from the serial port     
int i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte test;
byte zero;
byte a,b,c,d,e,f,g,h;
int luminosite = 10;
char  *Day[] = {
  "","Lun","Mar","Mer","Jeu","Ven","Sam","Dim"};
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

void setup()
{
  delay(50);
  Wire.begin();
  Serial.begin(57600); 
  zero=0x00;
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

    else if (command == 82 || command == 114) {      //If command = "Rr" Read Date ... BBR
      getDateDs1307();
      Serial.println(" ");
    }

    else if (command == 76 || command == 108) {      // if command = Ll" luminosite entre 0 et 15 
      delay(100);
      luminosite = Serial.parseInt();
      Serial.print("Luminosite reglee a :");
      Serial.println(luminosite);
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


  lc.setIntensity(0, luminosite);
  /* and clear the display */
  a=b=c=d=e=f=g=h = 0;

  bitSet(a,8-dayOfWeek);
  bitSet(b,8-(dayOfMonth/10));

  switch (dayOfMonth%10){
  case 0:
    bitSet(c,7);
    break;

  case 1:
    bitSet(c,6);
    break;

  case 2:
    bitSet(c,5);
    break;

  case 3:
    bitSet(b,4);
    break;

  case 4:
    bitSet(c,4);
    break;

  case 5:
    bitSet(b,3);
    break;

  case 6:
    bitSet(c,3);
    break;

  case 7:
    bitSet(b,2);
    break;

  case 8:
    bitSet(c,2);
    break;

  case 9:
    bitSet(c,1);
    break;
  }

  if (month < 9){
    bitSet(d,8 - month);
  } 
  else {
    switch(month){

    case 9:
      bitSet(e,0);
      break;

    case 10:
      bitSet(f,0);
      break;

    case 11:
      bitSet(g,0);
      break;

    case 12:
      bitSet(h,0);
      break;
    }
  }

  bitSet(e,8-hour/10);

  switch(hour%10){

  case 0:
    bitSet(e,5);
    break;

  case 1:
    bitSet(f,7);
    break;

  case 2:
    bitSet(g,7);
    break;

  case 3:
    bitSet(h,7);
    break;

  case 4:
    bitSet(f,6);
    break;

  case 5:
    bitSet(g,6);
    break;

  case 6:
    bitSet(h,6);
    break;

  case 7:
    bitSet(f,5);
    break;

  case 8:
    bitSet(g,5);
    break;

  case 9:
    bitSet(h,5);
    break;
  }

  switch(minute/10){

  case 0:
    break;

  case 1:
    bitSet(e,4);
    break;

  case 2:
    bitSet(e,3);
    break;

  case 3:
    bitSet(e,2);
    break;

  case 4:
    bitSet(e,1);
    break;

  case 5:
    bitSet(f,4);
    break;
  }

  switch(minute%10){

  case 0:
    bitSet(f,3);
    break;

  case 1:
    bitSet(f,2);
    break;

  case 2:
    bitSet(f,1);
    break;

  case 3:
    bitSet(g,4);
    break;

  case 4:
    bitSet(g,3);
    break;

  case 5:
    bitSet(g,2);
    break;

  case 6:
    bitSet(g,1);
    break;

  case 7:
    bitSet(h,4);
    break;

  case 8:
    bitSet(h,3);
    break;

  case 9:
    bitSet(h,2);
    break;
  }

  if ((millis()%1000)<500){
    bitSet(h,1);
  } 
  else {
  }

  lc.setColumn(0, 0, a);
  lc.setColumn(0, 1, b);
  lc.setColumn(0, 2, c);
  lc.setColumn(0, 3, d);
  lc.setColumn(0, 4, e);
  lc.setColumn(0, 5, f);
  lc.setColumn(0, 6, g);
  lc.setColumn(0, 7, h);


  delay(100);
}



