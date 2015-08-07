int ledb = 11;
int ledr = 10;
int ana = A0;
int no = 0;
int so = 0;
int sn = 0;

void setup(){
pinMode(ledb, OUTPUT);
pinMode(ledr, OUTPUT);

}

void loop(){
  no = analogRead(ana);
  so = map(no, 0, 1023, 0, 255);
  analogWrite(ledb, so);
  sn = map(so, 0, 255, 255, 0);
  analogWrite(ledr, sn);
  delay(1);
}

