int led1 = 9;
int led2 = 10;
int led3 = 11;
int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;
int b1 = 0;
int b2 = 0;
int b3 = 0;
int b4 = 0;
int bp1 = 0;
int bp2 = 0;
int bp3 = 0;
int bp4 = 0;


void setup() {
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  digitalWrite(button1, HIGH);
  digitalWrite(button2, HIGH);
  digitalWrite(button3, HIGH);
  digitalWrite(button4, HIGH);
  
}

void loop() {

  b1 = digitalRead(button1);
  b2 = digitalRead(button2);
  b3 = digitalRead(button3);
  b4 = digitalRead(button4);

  if (b1 == 0) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  } else {
  }

  if (b2 == 0 && b1 == 1) {
    digitalWrite(led1, HIGH);
  } else {
  }

  if (b3 == 0 && b1 == 1) {
    digitalWrite(led2, HIGH);
  } else {
  }

  if (b4 == 0 && b1 == 1) {
    digitalWrite(led3, HIGH);
  } else {
  }



}
