//Libraries
 #include <avr/wdt.h>
 #include <TimerOne.h>
 #include <Wire.h>
 #include <Arduino.h>
 #include <dht11.h>
 #include <digitalWriteFast.h>
// #include <EEPROM.h>
// #include <SPI.h>



//Pinout
 #define SIN_ROW 11
 #define CLK_ROW 13
 #define LE_ROW 10

 #define SIN_LED 4
 #define CLK_LED 7
 #define LE_LED 6

 #define OE 5

 #define CLK_DIS 2
 #define SIN_DIS 3
 #define LE_DIS 9

 #define DHT11PIN 8

 #define SWup A3
 #define SENSOR A2
 #define SWdown A1
 #define SWenter A0

 #define SCL A5
 #define SDA A4

//Subroutine defines
 #define I2C_WRITE Wire.write 
 #define I2C_READ  Wire.read
 #define DS1307_I2C_ADDRESS 0x68
 
 
//Colour def
 
//DHT start 
 dht11 DHT11; 

//variables
  byte zero = 0x00;
  char seconds, minutes, hours, date, day, month, year;
  
  // Adressing
     int RGB = 0b0000000000000000; 
  // Colours
     unsigned char Bsec,Bmin, Bhou;
     
int i,j,k,l;
int chk;
char  temp, hum;
char colcou, timcou;
int DScou;
char DIS[4];

char action, setmode;
boolean zeroFlag;

boolean disUpdateFlag, DHTupdateFlag, colUpdateFlag, fceUpdateFlag;                                                                            //FLAGS

int LUX;

boolean SecondsClearFlag, MinutesClearFlag, HoursClearFlag;
boolean ShowTime;
boolean DisTime, DisTemp, DisHum, DisDay, DisDate;

boolean timeSet = 0;

 unsigned char PWMcyc;
 boolean ShiftFlag;
 
unsigned char PWMfr;
char lastsec, lastmin, lasthou;
unsigned long curmilsw;

//LED display
char cisla1 [17] = {0b01111110, 0b00001100, 0b10110110, 0b10011110, //0123										  ASOCIACE CISEL NA DISPLEJ 1, 3 |0x01 - TECKA
                    0b11001100, 0b11011010, 0b11111010, 0b00001110, //4567
                    0b11111110, 0b11011110, 0b11101110, 0b11111000, //89ab
                    0b01110010, 0b10111100, 0b11110010, 0b11100010, //cdef
                    0b11110000};                                    //t
char cisla2 [17] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, //0123										  ASOCIACE CISEL NA DISPLEJ 2, 4 |0x80 - TECKA
                    0b01100110, 0b01101101, 0b01111101, 0b00000111, //4567
                    0b01111111, 0b01101111, 0b01110111, 0b01111100, //89ab
                    0b00111001, 0b01011110, 0b01111001, 0b01110001, //cdef
                    0b01111000};                                    //t


//colours
int RGBs[10][3] = {{0x0002, 0x0002, 0x0002},  //RED                                                               Barvy - R, G, B, W, Y, LG, PINK, VIO, TYRK, LB
                   {0x0004, 0x0004, 0x0004},  //GREEN
                   {0x0001, 0x0001, 0x0001},  //BLUE
                   {0x0001, 0x0002, 0x0004},  //WHITE
                   {0x0002, 0x0002, 0x0004},  //Yellow
                   {0x0002, 0x0004, 0x0004},  //Light Green  
                   {0x0002, 0x0002, 0x0001},  //Pink
                   {0x0002, 0x0001, 0x0001},  //Violet
                   {0x0004, 0x0004, 0x0001},  //Tyrkis
                   {0x0004, 0x0001, 0x0001}}; //Light Blue                              
int RGBm[10][3] = {{0x0008, 0x0008, 0x0008},		//																      0  1  2  3  4  5   6     7    8     9	
                   {0x0010, 0x0010, 0x0010},
                   {0x0020, 0x0020, 0x0020},
                   {0x0010, 0x0020, 0x0008},  
                   {0x0008, 0x0008, 0x0010},
                   {0x0008, 0x0010, 0x0010},
                   {0x0008, 0x0008, 0x0020},
                   {0x0008, 0x0020, 0x0020},
                   {0x0010, 0x0010, 0x0020},
                   {0x0010, 0x0020, 0x0020}};
int RGBh[10][3] = {{0x0040, 0x0040, 0x0040},
                   {0x0080, 0x0080, 0x0080},
                   {0x0100, 0x0100, 0x0100},
                   {0x0040, 0x0080, 0x0100},
                   {0x0040, 0x0040, 0x0080},
                   {0x0040, 0x0080, 0x0080},
                   {0x0040, 0x0040, 0x0100},
                   {0x0040, 0x0100, 0x0100},
                   {0x0080, 0x0080, 0x0100},
                   {0x0080, 0x0100, 0x0100}};

boolean secu[60];																								 // Jednotlive sekundy, minuty, hodiny			
boolean minu[60];
boolean hodi[60];

int datacas[12];

char fce;																										// Funkce zobrazeni (efekty) 

																												// INTERRUPT ROUTINE - mixovani barev. ISP jebalo, nemichalo modrou (asi kvuli SS)
void timerIsr(){

RGB = 0x0000; 

 unsigned char itt;
 
     
if  (PWMcyc > PWMfr) {colcou++; 
            if (colcou == 3) colcou = 0; PWMcyc = 0;
            RGB = (RGBs[Bsec][colcou] | RGBm[Bmin][colcou] | RGBh[Bhou][colcou]);
        digitalWriteFast(OE, LOW);
        digitalWriteFast(LE_ROW, LOW);

     

      for (itt = 0; itt < 16; itt++)  {
        digitalWriteFast(SIN_ROW, !!(RGB  & (1 << (15 - itt))));
          digitalWriteFast(CLK_ROW, HIGH);
          digitalWriteFast(CLK_ROW, LOW); }	
    
      digitalWriteFast(LE_ROW, HIGH);   
        
      }

    else {PWMcyc++; digitalWriteFast(OE, HIGH);}


}
                                                                                                              
void processDIS(){																								// Process DIS - priradit k DIS[0-3] hodnotu 
    
  if (seconds > 41 && seconds < 48) {
    DIS[3] = cisla2[12];
    DIS[2] = 0b11000110;
    DIS[1] = cisla2[temp%10];
    DIS[0] = cisla1[temp/10];}
    
  else if (seconds > 21 && seconds < 28) {
    DIS[3] =  0b01011100;
    DIS[2] =  0b11000110;
    DIS[1] =  cisla2[hum%10];
    DIS[0] =  cisla1[hum/10];}
  
  else if (seconds > 1 && seconds < 8) {
    DIS[0] =  cisla1[date/10];
    DIS[1] =  cisla2[date%10] | 0b10000000;
    DIS[2] =  cisla1[month/10];
    DIS[3] =  cisla2[month%10] | 0b10000000;}
 
  else { 
    DIS[3] = cisla2[minutes%10];
    DIS[2] = cisla1[minutes/10];
    DIS[1] = cisla2[hours%10];
    DIS[0] = cisla1[hours/10];}
}



                                                                                                               
void updateDIS(){																								//UPDATE DISpleje
  
  unsigned char itg;
  digitalWrite(LE_DIS, LOW);   
    for (itg = 0; itg < 8; itg++)  {
     digitalWriteFast(SIN_DIS, !!(DIS[3]  & (1 << (7 - itg))));
     digitalWriteFast(CLK_DIS, HIGH);
     digitalWriteFast(CLK_DIS, LOW); }
    for (itg = 0; itg < 8; itg++)  {
     digitalWriteFast(SIN_DIS, !!(DIS[2]  & (1 << (7 - itg))));
     digitalWriteFast(CLK_DIS, HIGH);
     digitalWriteFast(CLK_DIS, LOW); }
    for (itg = 0; itg < 8; itg++)  {
     digitalWriteFast(SIN_DIS, !!(DIS[1]  & (1 << (7 - itg))));
     digitalWriteFast(CLK_DIS, HIGH);
     digitalWriteFast(CLK_DIS, LOW); }
    for (itg = 0; itg < 8; itg++)  {
     digitalWriteFast(SIN_DIS, !!(DIS[0]  & (1 << (7 - itg))));
     digitalWriteFast(CLK_DIS, HIGH);
     digitalWriteFast(CLK_DIS, LOW); }
     
   digitalWrite(LE_DIS, HIGH);
   
}

void setTime(){
  if (digitalReadFast(SWenter) == 0 && digitalReadFast(SWup) == 0 && digitalReadFast(SWdown) == 0) zeroFlag = true;
  if (zeroFlag == true) {
    if (digitalReadFast(SWup) == 1) action = 1;
    else if (digitalReadFast(SWdown) == 1) action = 2;
    else if (digitalReadFast(SWenter) == 1) setmode++;
    else action = 0;
    }
    
    
    switch (setmode) {
     case 0:{ if (action == 1) hours++;
              if (action == 2) hours--;
              hours = constrain (hours, 0, 23);
              DIS[3] = 0;
              DIS[2] = 0;
              DIS[1] = cisla2[hours%10];
              DIS[0] = cisla1[hours/10];
              break; }
     case 1:{ if (action == 1) minutes++;
              if (action == 2) minutes--;
              minutes = constrain (minutes, 0, 59);
              DIS[1] = 0;
              DIS[0] = 0;
              DIS[3] = cisla2[minutes%10];
              DIS[2] = cisla1[minutes/10];
              break; }
     case 2:{ if (action == 1) date++;
              if (action == 2) date--;
              date = constrain (date, 1, 31);
              DIS[3] = 0;
              DIS[2] = 0;
              DIS[1] = cisla2[date%10];
              DIS[0] = cisla1[date/10];
              break; }
     case 3:{ if (action == 1) month++;
              if (action == 2) month--;
              month = constrain (month, 1, 12);
              DIS[1] = 0;
              DIS[0] = 0;
              DIS[3] = cisla2[month%10];
              DIS[2] = cisla1[month/10];
              break; }
     case 4:{ if (action == 1) year++;
              if (action == 2) year--;
              year = constrain (year, 0, 99);
              DIS[0] =  cisla1[2];
              DIS[1] =  cisla2[0] ;
              DIS[2] =  cisla1[year/10];
              DIS[3] =  cisla2[year%10];
              break; }
     case 5:  {setDateDs1307(); seconds = 0; timeSet = 0;
              break;}
}

while (digitalReadFast(SWenter) == 1);;
while (digitalReadFast(SWup) == 1);;
while (digitalReadFast(SWdown) == 1);;
}          
              
              
     
    
    
  
                                                                                                                  
void writeTime(){																								//Write Time
  
  char it; digitalWriteFast(LE_LED, LOW);    
  for (it = 12; it > 0; it--){
          k = (it-1)*5;
           digitalWriteFast(SIN_LED, secu[k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, minu[k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, hodi[k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, secu[1+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, minu[1+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, hodi[1+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, secu[2+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, minu[2+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, secu[4+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, minu[4+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, hodi[4+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, hodi[3+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, minu[3+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, secu[3+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, hodi[2+k]); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
           digitalWriteFast(SIN_LED, 0); digitalWriteFast(CLK_LED, HIGH); digitalWriteFast(CLK_LED, LOW);
       }
       digitalWriteFast(LE_LED, HIGH);
  
  }
                                                                                                              
void DHTread(){																								   	//DHT Read: temp, hum
 Timer1.stop();  // vypnout interrupt - nenacitalo
  chk = DHT11.read(DHT11PIN);
  if (chk == 0) {
  temp = char((float)DHT11.temperature);
  hum  = char((float)DHT11.humidity);
 }
Timer1.resume();  // zapnout interrupt
}
                                                                                                              
byte decToBcd(byte val){																			    		//DS1307 communication
  return ( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val){
  return ( (val/16*10) + (val%16) );
}


void setDateDs1307()                
{  Wire.beginTransmission(DS1307_I2C_ADDRESS);
   I2C_WRITE(zero);
   I2C_WRITE(decToBcd(seconds) & 0x7f);    // 0 in bit 7 starts the clock
   I2C_WRITE(decToBcd(minutes));
   I2C_WRITE(decToBcd(hours));      
   I2C_WRITE(decToBcd(day));
   I2C_WRITE(decToBcd(date));
   I2C_WRITE(decToBcd(month));
   I2C_WRITE(decToBcd(year));
   Wire.endTransmission();
}

void getDateDs1307()
{
  
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  Wire.endTransmission();
 
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
                                               
  seconds    = bcdToDec(I2C_READ() & 0x7f);       // &control bits
  minutes    = bcdToDec(I2C_READ());
  hours      = bcdToDec(I2C_READ() & 0x3f);
  day        = bcdToDec(I2C_READ());
  date       = bcdToDec(I2C_READ());
  month      = bcdToDec(I2C_READ());
  year       = bcdToDec(I2C_READ());
  
  
  }
 

 // CLEAR CLOCK
void clearClock(){
  
  clearTime();
  DIS[0] = 0; DIS[1] = 0; DIS[2] = 0; DIS[3] = 0; 
  writeTime();
  updateDIS();
 }
																												// CLEAR TIME / Seconds/ Minutes / Hours
void clearTime(){
  for (i=0; i<60; i++){
   secu[i] = 0; minu[i] = 0; hodi[i] = 0;}
  }
  
void clearSeconds(){
   for (i=0; i<60; i++){
   secu[i] = 0; }
  }
  
void clearMinutes(){
  for (i=0; i<60; i++){
   minu[i] = 0;}
  }
  
void clearHours(){
  for (i=0; i<60; i++){
   hodi[i] = 0;}
  }

void assignLED(){																								// prirazeni LED pro cas
  
  char itb, hourst;
if (fce == 0){
  
  clearTime(); 
  secu[seconds] = 1; 
  if (minutes < 59 && minutes > 0) {
    minu[minutes] = 1;
    minu[minutes-1] = 1;
    minu[minutes+1] = 1;}
    else if (minutes == 59){
    minu[58] = 1;
    minu[59] = 1;
    minu[0] = 1;}
    else {
    minu[59] = 1;
    minu[0] = 1;
    minu[1] = 1;} 
   
   if (hours > 11) hourst = (hours - 12)*5; else hourst = hours*5; 
   if (hourst > 0) {
    hodi[hourst] = 1;
    hodi[hourst-1] = 1;
    hodi[hourst+1] = 1;
    hodi[hourst-2] = 1;
    hodi[hourst+2] = 1;}
      else {
    hodi[58] = 1;
    hodi[59] = 1;
    hodi[0] = 1;
    hodi[1] = 1;
    hodi[2] = 1;}
}

else if (fce == 1) {
    clearTime(); 
    if (hours > 11) hourst = (hours - 12)*5; else hourst = hours*5;     
    if (seconds == 0) {clearSeconds(); secu[0] = 1;}
    if (minutes == 0) {clearMinutes(); minu[0] = 1;}
    if (hourst  == 0) {clearHours(); hodi[0] = 1;}
    
    for (itb = 0; itb <= seconds; itb++) {secu[itb] = 1;}
    for (itb = 0; itb <= minutes; itb++) {minu[itb] = 1;}
    for (itb = 0; itb <= (hourst + (minutes/15)); itb++) hodi[itb] = 1;
}

else if (fce == 2) {
  clearTime(); 
  if (hours > 11) hourst = (hours - 12)*5; else hourst = hours*5;  
  if (minutes == 0) {clearMinutes(); minu[0] = 1;}
  if (hourst  == 0) {clearHours(); hodi[0] = 1;}
  
  for (itb = 0; itb < 12; itb++) {
      secu[(itb*5)] = 1; }
  
  for (itb = 0; itb <= minutes; itb++) {minu[itb] = 1;}
  for (itb = 0; itb <= (hourst + (minutes/15)); itb++) hodi[itb] = 1;

} 
}
 
void adjustLUX (){                                                                                         		// BRIGHTNESS

 if (LUX > 1000) PWMfr = 0;
 else if (LUX > 980) PWMfr = 0;
 else if (LUX > 960) PWMfr = 2;
 else if (LUX > 940) PWMfr = 4;
 else if (LUX > 920) PWMfr = 6;
 else if (LUX > 900) PWMfr = 8;
 else if (LUX > 880) PWMfr = 10;
 else if (LUX > 800) PWMfr = 12;
 else if (LUX > 700) PWMfr = 15;
 else if (LUX > 600) PWMfr = 20;
 else if (LUX > 500) PWMfr = 30;
 else if (LUX > 400) PWMfr = 40;
 else if (LUX > 300) PWMfr = 50;
 else if (LUX < 300) PWMfr = 60;
 

 
} 
                                                                                                                              
void setup (){																									//SETUP

// Pin Directions  
pinMode(SIN_ROW, OUTPUT);
pinMode(SIN_LED, OUTPUT);
pinMode(SIN_DIS, OUTPUT);
pinMode(CLK_ROW, OUTPUT);
pinMode(CLK_LED, OUTPUT);
pinMode(CLK_DIS, OUTPUT);
pinMode(LE_ROW, OUTPUT);
pinMode(LE_LED, OUTPUT);
pinMode(LE_DIS, OUTPUT);
pinMode(OE, OUTPUT);
pinMode(SENSOR, INPUT);
pinMode(SWdown, INPUT);
pinMode(SWup, INPUT);
pinMode(SWenter, INPUT);
digitalWrite(OE, LOW);

// Init
wdt_enable(WDTO_1S);                                //  watchdog

Wire.begin();
Wire.beginTransmission(DS1307_I2C_ADDRESS);            
   I2C_WRITE(0x07);
   I2C_WRITE(0b00010000);
Wire.endTransmission();   

// Get Time
getDateDs1307(); 
setDateDs1307(); // start Clock

//CLear Clock
clearClock();
Bsec = 1; Bhou = 2;
//Set Initial PWM - was messing


//Setup Interrupt for colour refresh
Timer1.initialize(100);                // set a timer of length 10000 microseconds = 100Hz, 3000 = 333Hz
Timer1.attachInterrupt( timerIsr );     // Run Interrupt service routine timerIsr on interrupt
DHTread();  
/*
//SPI
SPI.setDataMode(SPI_MODE0);
SPI.setClockDivider(SPI_CLOCK_DIV2);
SPI.setBitOrder(MSBFIRST);
SPI.begin();*/


PWMfr = 50;                                  // max 60 to avoid flicker
fce = 2;
}
                                                                                                                
void loop(){																								    //LOOP
  
   wdt_reset();
 
  if (seconds == 0 && DHTupdateFlag == 0) {DHTread(); DHTupdateFlag = 1;}
  if (seconds == 1) {DHTupdateFlag = 0;}
  
   
  if (seconds%10 == 0 && colUpdateFlag == 0) {
     Timer1.stop();
     Bsec++;
     Bmin++;
     Bhou++;
     if (Bsec > 9) Bsec = 0; 
     if (Bmin > 9) Bmin = 0; 
     if (Bhou > 9) Bhou = 0;
     Timer1.resume();
     adjustLUX();
     colUpdateFlag = 1;
  }
     
     
  if (seconds%10 !=0) colUpdateFlag = 0;
  if (lastmin != minutes)  {
        fce ++; 
        lastmin = minutes; 
        fceUpdateFlag = 1; 
        if (fce == 3) fce = 0; 
  } else fceUpdateFlag = 0;
    
    
 
     if (digitalReadFast(SWenter) == 1 && timeSet == 0) {timeSet = 1; DScou = 0;}
     if (timeSet == 0) DScou++;
     if (timeSet == 1) {setTime(); updateDIS();}
  
     assignLED();
     writeTime();
     
     if (DScou == 1) LUX = analogRead(SENSOR); 
     if (DScou == 4) processDIS();
     if (DScou == 5) updateDIS();
     if (DScou == 6) {getDateDs1307(); DScou = 0;}
     
     
      
     /* updateDIS();
      writeTime();*/
      
  
            
 
  
  
  
  

    


  

    
   
  
 
 

}
