#include <LiquidCrystal.h>

int msn = 0;  // ms net renvoi les 3 chiffes des ms ex: 234ms pour 24234ms bruts
unsigned long msb = 0; // ms Bruts (totals ex 24234ms)

unsigned long s = 0; // renvois la valeur brut des secondes ex: 600 pour 10 minutes
int sn = 0; // renvois la valeur net des secondes ex: pour 67 renvois 7
unsigned long m = 0; // renvois la valeur brut des minutes ex: 600 minutes pour 10 heures
int mn = 0; // renvois la valeur net des minutes ex: pour 67 renvois 7
unsigned long h = 0; // renvois la valeur brut des heures ex: 48 minutes pour 2 jours
int hn = 0; // renvois la valeur net des jours ex: pour 49 renvois 1
int j = 0; // renvois la valeurs des jours ne peut pas depasser les 50 jours (limites arduino)
int led = 13;
LiquidCrystal lcd(0, 1, 2, 3, 4, 5); // LCD en 4 bits 0 a RS, 1 a E, 2 a D4, 3 a D5, 4 a D6, et 5 a D7

void setup() {

  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  lcd.setCursor(2, 0);
  lcd.print("j");
  lcd.setCursor(5, 0);
  lcd.print("h");
  lcd.setCursor(8, 0);
  lcd.print("m");
  lcd.setCursor(11, 0);
  lcd.print("s");
  lcd.setCursor(15, 0);
  lcd.print("m");

}

void loop() {

  digitalWrite(led, HIGH);
  
  msb = millis();
  msn = msb % 1000;
  s = msb / 1000;
  sn = s % 60;
  m = s / 60;
  mn = m % 60;
  h = m / 60;
  hn = h % 24;
  j = h / 24;

  if (j < 10) {
    lcd.setCursor(0, 0);
    lcd.print("0");
    lcd.setCursor(1, 0);
  }
  else {

    lcd.setCursor(0, 0);
    lcd.print(j);
  }
  lcd.print(j);

  if (hn < 10) {
    lcd.setCursor(3, 0);
    lcd.print("0");
    lcd.setCursor(4, 0);
  }
  else {
    lcd.setCursor(3, 0);
  }
  lcd.print(hn);


  if (mn < 10) {
    lcd.setCursor(6, 0);
    lcd.print("0");
    lcd.setCursor(7, 0);
  }
  else {
    lcd.setCursor(6, 0);
  }
  lcd.print(mn);

  if (sn < 10) {
    lcd.setCursor(9, 0);
    lcd.print("0");
    lcd.setCursor(10, 0);
  }
  else {
    lcd.setCursor(9, 0);
  }
  lcd.print(sn);

  if (msn < 100) {
    lcd.setCursor(12, 0);
    lcd.print("0");
    lcd.setCursor(13, 0);
  }
  else {
    lcd.setCursor(12, 0);
  }
  lcd.print(msn);

  digitalWrite(led, LOW);

  delay(10);


}








