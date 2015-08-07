
int ledr = 12;
int ledb = 13;
int bop = 2;
int bom = 4;
int bm = 0;
int bp = 0;
int pa = 2;
int pe = 1000;

void setup(){
pinMode(ledr, OUTPUT);
pinMode(ledb, OUTPUT);
pinMode(bop, INPUT);
pinMode(bom, INPUT);
}

void loop(){
bm = digitalRead(bom);
bp = digitalRead(bop);

if (bm == HIGH) {
pe = pe - pa;
} else {
  delay (0);
}

if (bp == HIGH) {
pe = pe + pa ;
} else {
}

if (pe < 10){
  pe = pe + pa;
  
} else {}

if (pe > 100){
  pe = 100;
} else {}
  
  pa = pe / 10;
  
  Serial.print(pe);
  
digitalWrite(ledr, HIGH);
delay(pe);
digitalWrite(ledr, LOW);
digitalWrite(ledb, HIGH);
delay(pe);
digitalWrite(ledb, LOW);

}


