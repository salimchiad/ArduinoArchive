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
int r = 0;
int b = 0;
int g = 0;
int rp = 0;
int bp = 0;
int gp = 0;


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

  bp1 = b1;
  bp2 = b2;
  bp3 = b3;
  bp4 = b4;

  b1 = digitalRead(button1);
  b2 = digitalRead(button2);
  b3 = digitalRead(button3);
  b4 = digitalRead(button4);

  if (b1 == 0 && bp1 == 1) {
    bp = 0;
    gp = 0;
    rp = 0;
  } else {
  }

  if (b2 == 0 && bp2 == 1) {
    rp = rp + 1;
  } else {
  }

  if (b3 == 0 && bp3 == 1) {
    gp = gp + 1;
  } else {
  }

  if (b4 == 0 && bp4 == 1) {
    bp = bp + 1;
  } else {
  }

  if (rp == 11) {
    rp = 0;
  } else {
  }

  if (gp == 11) {
    gp = 0;
  } else {
  }

  if (bp == 11) {
    bp = 0;
  } else {
  }

  r = map(rp, 0, 10, 0, 255);
  g = map(gp, 0, 10, 0, 255);
  b = map(bp, 0, 10, 0, 255);

  analogWrite(led1, r);
  analogWrite(led2, g);
  analogWrite(led3, b);


}
