int capteur = 0;
int led1 = 10;
int led2 = 9;
int led3 = 13;
int hp = 8;
int ledd1;
int ledd2;
int retour;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {

  retour = analogRead(capteur) / 10;
  ledd1 = map(retour, 19, 66, 0, 255);
  ledd2 = map(retour, 19, 66, 255, 0);
  analogWrite(led1, ledd1);
  analogWrite(led2, ledd2);
  if (ledd1 > 180) {
    digitalWrite(led3, HIGH);
    tone(hp, 5000);
  } else {
    digitalWrite(led3, LOW);
    noTone(hp);
  }
  
  Serial.print("Retour Capteur : ");
  Serial.println(retour);  

  delay(500);
}
