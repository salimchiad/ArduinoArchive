int hp = 8;
int potar = A0;
unsigned int freq;
unsigned int entree;
int btA1, btA2, btA3, btA4;

void setup() {
  // put your setup code here, to run once:
  pinMode(hp, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  entree = analogRead(potar);
  freq = map(entree, 0, 1023, 0, 2000);
  Serial.println(freq);
  tone(hp, freq);
  delay(1);

}
