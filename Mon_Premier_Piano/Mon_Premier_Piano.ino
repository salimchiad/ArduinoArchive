int hp = 8;
int bt1 = 2;
int bt2 = 3;
int bt3 = 4;
int bt4 = 5;
int btA1, btA2, btA3, btA4;

void setup() {
  // put your setup code here, to run once:
  pinMode(hp, OUTPUT);
  pinMode(bt1, INPUT);
  pinMode(bt2, INPUT);
  pinMode(bt3, INPUT);
  pinMode(bt4, INPUT);
  digitalWrite(bt1, HIGH);
  digitalWrite(bt2, HIGH);
  digitalWrite(bt3, HIGH);
  digitalWrite(bt4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  btA1 = digitalRead(bt1);
  btA2 = digitalRead(bt2);
  btA3 = digitalRead(bt3);
  btA4 = digitalRead(bt4);

  if (btA1 == HIGH && btA2 == HIGH && btA3 == HIGH && btA4 == HIGH) {
    noTone(hp);
  } else {

    if (btA1 == LOW) {
      tone(hp, 2000);
    }

    if (btA2 == LOW) {
      tone(hp, 3000);
    }

    if (btA3 == LOW) {
      tone(hp, 4000);
    }

    if (btA4 == LOW) {
      tone(hp, 5000);
    }


  }
}
