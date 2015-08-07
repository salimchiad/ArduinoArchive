int led1 = 9;
int led2 = 10;
int led3 = 11;
int led4 = 2;
int lum1 = 0;
int lum2 = 0;
int l1 = 0;
int l2 = 0;
int l0 = 0;
int diff = 0;

void setup() {
  
  Serial.begin(115200);
  
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  pinMode (led4, OUTPUT);
}

void loop() {

  l1 = analogRead(A0);
  l2 = analogRead(A1);
  l0 = (l1 + l2) / 2;

  if (l1 == l2) {
    digitalWrite(led4, HIGH);
  } else {
    digitalWrite(led4, LOW);
  }


  lum1 = map(l0, 0, 1023, 0, 255);
  lum2 = map(l0, 0, 1023, 255, 0);

  Serial.print("L1 =");
  Serial.print(l1);
  Serial.print(", L2 =");
  Serial.println(l2);
  

  analogWrite(led3, lum1);
  analogWrite(led2, lum2);
  delay(10);

}
