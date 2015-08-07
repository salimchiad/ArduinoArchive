int led1 = 9;
int led2 = 10;
int led3 = 11;
int pot1 = 0;
int pot2 = 0;
int pot3 = 0;
int lum1 = 0;
int lum2 = 0;
int lum3 = 0;

void setup() {
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  Serial.begin(9600);
  
  }

void loop() {
 
  pot1 = analogRead(A0);
  pot2 = analogRead(A1);
  pot3 = analogRead(A2);
  
  lum1 = map(pot1, 0, 1023, 0, 255);
  lum2 = map(pot2, 0, 1023, 0, 255);
  lum3 = map(pot3, 0, 1023, 0, 255);
  
  analogWrite(led1, lum1);
  analogWrite(led2, lum2);
  analogWrite(led3, lum3);
  
  Serial.print("Bleu ");
  Serial.print(lum1/2.55);
  Serial.print("  Vert ");
  Serial.print(lum2/2.55);
  Serial.print("  Rouge ");
  Serial.println(lum3/2.55);
  delay(500);

}
