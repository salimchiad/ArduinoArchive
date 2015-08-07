#include <Wire.h>

int adresse = 0; // adresse choisir 0, 1, 2, ou 3.
int AdresseA, AdresseB, AdresseC;
int EtatA, EtatB, EtatC;
int LedA = 13;
int LedB = 12;
int LedC = 11;
int etat = LOW;
int bouton = 6;

void setup() {
  Wire.begin(adresse);
  Wire.onRequest(Lecture);
  AdresseA = ((adresse + 1) % 4);
  AdresseB = ((adresse + 2) % 4);
  AdresseC = ((adresse + 3) % 4);
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(LedC, OUTPUT);
  pinMode(bouton, INPUT);
  delay(adresse);
}

void loop() {
  
  delay(10);
  
  EtatA = LOW;
  EtatB = LOW;
  EtatC = LOW;

  Wire.requestFrom(AdresseA, 1);
  while (Wire.available())
  {
    EtatA = Wire.read();
  }
  digitalWrite(LedA, EtatA);

  Wire.requestFrom(AdresseB, 1);
  while (Wire.available())
  {
    EtatB = Wire.read();
  }
  digitalWrite(LedB, EtatB);

  Wire.requestFrom(AdresseC, 1);
  while (Wire.available())
  {
    EtatC = Wire.read();
  }
  digitalWrite(LedC, EtatC);

  etat = digitalRead(bouton);
  
}



void Lecture()
{
  Wire.write(etat);
}
