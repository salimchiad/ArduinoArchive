int i = 0;
char lu = ' ';
int LED = 13;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {

  if (Serial.available())
  {
    lu = Serial.read();
    if (lu == '1')
    {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
    lu = ' ';
  }
}
