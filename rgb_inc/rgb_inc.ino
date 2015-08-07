int r;
int v;
int b;
int x;
int y;
int z;

const int ledb = 6;
const int ledv = 5;
const int ledr = 3;



void setup(){
  pinMode(ledr, OUTPUT);
  pinMode(ledv, OUTPUT);
  pinMode(ledb, OUTPUT);
}

void loop(){
  analogWrite(ledr, r);
  analogWrite(ledv, v);
  analogWrite(ledb, b);
  
  for (r=0;r<255;r++){
  analogWrite(ledr, r);
  analogWrite(ledv, v);
  analogWrite(ledb, b);
  delay(1);
}
for (v=0;v<255;v++){
  analogWrite(ledr, r);
  analogWrite(ledv, v);
  analogWrite(ledb, b);
  delay(1);
}
for (b=0;b<255;b++){
  analogWrite(ledr, r);
  analogWrite(ledv, v);
  analogWrite(ledb, b);
  delay(1);
}


}




