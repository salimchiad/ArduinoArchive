byte nombre = 0;
unsigned int compteur = 6;
byte Etat;
//byte i;

void setup() {                
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  compteur++;
  nombre = compteur % 6;
  nombre = nombre + 1;
  Etat = digitalRead(4);
  
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(1, LOW);
  digitalWrite(0, LOW);

  if (Etat == LOW) {
    if (nombre > 5) {
      digitalWrite(3, HIGH);
    } 
    else {
      digitalWrite(3, LOW);
    }

    if (nombre > 3) {
      digitalWrite(2, HIGH);
    } 
    else {
      digitalWrite(2, LOW);
    }

    if (nombre > 1) {
      digitalWrite(1, HIGH);
    } 
    else {
      digitalWrite(1, LOW);
    }

    if ((nombre % 2) == 1) {
      digitalWrite(0, HIGH);
    } 
    else {
      digitalWrite(0, LOW);
    }
    delay(5000);
  }
}












