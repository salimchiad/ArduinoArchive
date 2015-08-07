

#include <LiquidCrystal.h>

LiquidCrystal lcd(0, 1, 2, 3, 4, 5);
float temp = 0;
int term = A0;
float tempc = 0;
int tens = 0.001;

void setup(){
  lcd.begin(16, 2);
}

void loop(){

  lcd.setCursor(0, 0); // ne fonctionne pas donne pas de température logique
  lcd.print("Temperature");
  temp = analogRead(term);
  tempc = temp/2,0 ;
  lcd.setCursor(2, 1);
  lcd.print( tempc );
  lcd.setCursor(7, 1);
  lcd.write("C");
  tens = temp;
  lcd.setCursor(10, 1);
  tens = temp/1023*5000;
  lcd.print(tens);
  lcd.setCursor(14, 1);
  lcd.write("mV");
  delay(100);
}


