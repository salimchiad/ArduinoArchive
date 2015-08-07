

#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int ledb = 3;
int ledr = 5;
int ana = A0;
int no = 0;
int so = 0;
int sn = 0;

void setup(){
pinMode(ledb, OUTPUT);
pinMode(ledr, OUTPUT);
}

void loop(){
  
  lcd.begin(16, 2);
  no = analogRead(ana);
  so = map(no, 0, 1023, 0, 255);
  analogWrite(ledb, so);
  sn = map(so, 0, 255, 255, 0);
  analogWrite(ledr, sn);
  lcd.setCursor(0, 0);
  lcd.print("Rouge");
  lcd.setCursor(7, 0);
  lcd.print("   ");
  lcd.print(sn);
  lcd.setCursor(0, 1);
  lcd.print("Bleue");
  lcd.setCursor(7, 1);
  lcd.print("   ");
  lcd.print(so);
  delay(1000);
}

