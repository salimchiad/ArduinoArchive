int lum, temps;
int L1, L2, L3;
int led1 = 9;
int led2 = 10;
int led3 = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(temps);
  lum = analogRead(A0);
  Serial.println(lum);
  temps = map(lum, 300, 750, 10, 1000);
  L1 = random(0, 255);
  L2 = random(0, 255);
  L3 = random(0, 255);
  analogWrite(led1, L1);
  analogWrite(led2, L2);
  analogWrite(led3, L3);
  Serial.print("temps ");
  Serial.println(temps);
}
