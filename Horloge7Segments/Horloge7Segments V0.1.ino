//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

unsigned char chu = 0; // unite du nombre
unsigned char chd = 0; // dizaine du nombre
unsigned char cou = 255; // code de l'unite
unsigned char cod = 255; // code de la dizaine
unsigned char com = 0; // compteur
unsigned char t = 0;   // chiffre a traiter
int msn = 0;  // ms net renvoi les 3 chiffes des ms ex: 234ms pour 24234ms bruts
unsigned long msb = 0; // ms Bruts (totals ex 24234ms)
unsigned long ms = 0; // ms + décalage (qui est l'heure exacte)
unsigned long s = 0; // renvois la valeur brut des secondes ex: 600 pour 10 minutes
int sn = 0; // renvois la valeur net des secondes ex: pour 67 renvois 7
unsigned long m = 0; // renvois la valeur brut des minutes ex: 600 minutes pour 10 heures
int mn = 0; // renvois la valeur net des minutes ex: pour 67 renvois 7
unsigned long h = 0; // renvois la valeur brut des heures ex: 48 minutes pour 2 jours
int hn = 0; // renvois la valeur net des jours ex: pour 49 renvois 1
int j = 0; // renvois la valeurs des jours ne peut pas dépasser les 50 jours (limites arduino)
unsigned long decalage = 78510000;
int affdb = 0;
int afftpb = 0;
int affpb = 0;
int aff = 0;
byte z = 0;
unsigned int cl = 0;




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

 // Début comptage pour affichage
  if (z == 0) {
    affdb = millis();
    z = 1;
  }


 // Lire l'heure

  ms = millis() + decalage;
  msb = millis();
  s = ms / 1000;
  sn = s % 60;
  m = s / 60;
  mn = m % 60;
  h = m / 60;
  hn = h % 24;

 // Selection de l'affichage
 
  afftpb = millis() - affdb;

  if ( afftpb > 2000 ) {
    affdb = millis();
    aff = aff + 1;
    if ( aff > 2 ) {
      aff = 0;
    }
  }
 // Affichage des heures
  if (aff == 0) {
    com = hn;
  }
 // Affichage des minutes
  if (aff == 1) {
    com = mn;
  }
 // Affichage des secondes
  if (aff == 2) {
    com = sn;
  }

  chu = com % 10;
  chd = com / 10;

 //Convertion du nombre en code pour affichage

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

 // Clignotement du point indicateur de l'unité
 
  afftpb = millis() % 1000;

  if ( aff == 0) {
    cl = afftpb % 1000;
    if (cl < 500) {
      bitWrite(cou, 7, 1);
    } else {
      bitWrite(cou, 7, 0);
    }
  }

  if ( aff == 1) {
    cl = afftpb % 333;
    if (cl < 166) {
      bitWrite(cou, 7, 1);
    } else {
      bitWrite(cou, 7, 0);
    }
  }

  if ( aff == 2) {
    cl = afftpb % 166;
    if (cl < 83) {
      bitWrite(cou, 7, 1);
    } else {
      bitWrite(cou, 7, 0);
    }
  }

// envoi du code a l'affichage

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, cou);
  shiftOut(dataPin, clockPin, MSBFIRST, cod);
  digitalWrite(latchPin, HIGH);

}








