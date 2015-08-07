int ledr = 13;
int ledg = 12;
int ledb = 11;
int bop = 9;
int bom = 8;
int pe = 1000;
int pb = 4;
int pm = 4;
int pbc = 0;
int pmc = 0;
int po = 0;
int et = 0;


void setup(){
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(bop, INPUT);
  pinMode(bom, INPUT);
}

void loop(){
  
  delay(1);
  
  if (po > pe) {
    po = 0;
    if (et == 0 ) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledg, LOW);
      et = 1;
    } else {
      digitalWrite(ledr, LOW);
      digitalWrite(ledg, HIGH);
      et = 0;
    } 
  }else {
    
    }  
  
pm = digitalRead(bom);
pb = digitalRead(bop);

if (pb == HIGH){
  pbc = pbc + 1;
} else {
  pbc = 0;
}

if (pm == HIGH){
  pmc = pmc + 1;
} else {
  pmc = 0;
}


pe = pe + pbc;
pe = pe - pmc;
}



