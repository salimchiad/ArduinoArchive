int nomb = 0;
int led1 = 10;
int led2 = 9;
int ledd1;
int ledd2;
int nom;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {

  nom = analogRead(nomb) / 10;
  ledd1 = map(nom, 19, 66, 0, 255);
  ledd2 = map(nom, 19, 66, 255, 0);
  analogWrite(led1, ledd1);
  analogWrite(led2, ledd2);
  delay(1);
}
