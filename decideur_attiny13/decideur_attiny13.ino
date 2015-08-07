byte compteur;
int Etat;


void setup() {

#define bouton 4;
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
}

void loop() {
  compteur++;
  Etat = digitalRead(4);
  digitalWrite(1, LOW);
  digitalWrite(0, LOW);

  if (Etat == LOW) {
    if((compteur % 2) == 1){
      digitalWrite(0, HIGH);
    } 
    else {
      digitalWrite(1, HIGH);
    }
    delay(5000);
  }
}




