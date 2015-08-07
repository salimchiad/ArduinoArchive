int i = 0;
int etatBouton = LOW;
int bouton = 7;

void setup() {
  Serial.begin(9600);
  pinMode(bouton, INPUT);
}

void loop() {

  etatBouton = digitalRead(bouton);

  if (etatBouton == HIGH)
  {
    Serial.print("1");
  } else {
    Serial.print("0");
  }

delay(10);

}
