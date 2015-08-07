int led = 13;
int bouton = 5;
int etat_bouton = 0;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(bouton, INPUT);
}

void loop() {
  etat_bouton = digitalRead(bouton);

  if (etat_bouton == HIGH)
  {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
