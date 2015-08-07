//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int pota;
int potb;



void setup() {
  // put your setup code here, to run once:
  pinMode( dataPin, OUTPUT );
  pinMode( clockPin, OUTPUT );
  pinMode( latchPin, OUTPUT );
}

void loop() {


  pota = analogRead(A0);
  potb = analogRead(A1);

  d = map(pota, 0, 1023, 0, 5);
  c = map(potb, 0, 1023, 0, 7);
  
  a = 255;
  b = 0;

  bitClear(a, c);
  bitSet(b, d);


  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, a);
  shiftOut(dataPin, clockPin, MSBFIRST, b);
  digitalWrite(latchPin, HIGH);

}

