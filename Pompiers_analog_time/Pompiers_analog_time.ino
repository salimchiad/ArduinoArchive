
int ledb = 11;
int ledr = 10;
int no = 1000;
int ana = A0;
int so = 0;

void setup(){
pinMode(ledr, OUTPUT);
pinMode(ledb, OUTPUT);
}

void loop(){

no = analogRead(ana);
so = map(no, 0, 1023, 0, 10000);
digitalWrite(ledr, HIGH);
delay(so + 1);
digitalWrite(ledr, LOW);
digitalWrite(ledb, HIGH);
delay(so + 1);
digitalWrite(ledb, LOW);

}


