//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

unsigned char chu = 0; // unite du nombre
unsigned char chd = 0; // dizaine du nombre
unsigned char cou = 0; // code de l'unite
unsigned char cod = 0; // code de la dizaine
unsigned char com = 0; // compteur
unsigned char t = 0;   // chiffre a traiter

byte a = 0;

void setup() {
  pinMode( dataPin, OUTPUT );
  pinMode( clockPin, OUTPUT );
  pinMode( latchPin, OUTPUT );


  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, cou);
  shiftOut(dataPin, clockPin, MSBFIRST, cod);
  digitalWrite(latchPin, HIGH);
  delay(1000);
}

void loop() {


  for (com = 0; com < 100; com++) {

    chu = com % 10;
    chd = com / 10;


    for (a = 0; a < 2; a++) { // 0 unites 1 dizaines

      if (a == 0) {
        t = chu;
      } else {
        t = chd;
      }

      if (t == 0) {
        if ( a == 0) {
          cou = 63;
        } else {
          cod = 63;
        }
      }

      if (t == 1) {
        if ( a == 0) {
          cou = 6;
        } else {
          cod = 6;
        }
      }

      if (t == 2) {
        if ( a == 0) {
          cou = 91;
        } else {
          cod = 91;
        }
      }

      if (t == 3) {
        if ( a == 0) {
          cou = 79;
        } else {
          cod = 79;
        }
      }

      if (t == 4) {
        if ( a == 0) {
          cou = 102;
        } else {
          cod = 102;
        }
      }

      if (t == 5) {
        if ( a == 0) {
          cou = 109;
        } else {
          cod = 109;
        }
      }

      if (t == 6) {
        if ( a == 0) {
          cou = 125;
        } else {
          cod = 125;
        }
      }

      if (t == 7) {
        if ( a == 0) {
          cou = 7;
        } else {
          cod = 7;
        }
      }

      if (t == 8) {
        if ( a == 0) {
          cou = 127;
        } else {
          cod = 127;
        }
      }


      if (t == 9) {
        if ( a == 0) {
          cou = 111;
        } else {
          cod = 111;
        }
      }

    }


    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, cou);
    shiftOut(dataPin, clockPin, MSBFIRST, cod);
    digitalWrite(latchPin, HIGH);
    delay(1000);
  }
}
