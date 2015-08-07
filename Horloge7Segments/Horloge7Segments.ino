//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

unsigned char chu = 0; // unite du nombre
unsigned char chd = 0; // dizaine du nombre
unsigned char cou = 255; // code chiffre a affiché
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
unsigned long decalage = 0;
int affdb = 0;
int afftpb = 0;
int affpb = 0;
int aff = 0;
byte z = 0;
unsigned int cl = 0;
int a = 0;
int bthp = 0;
int bthn = 0;
int btmp = 0;
int btmn = 0;


void setup() {
  pinMode( dataPin, OUTPUT );
  pinMode( clockPin, OUTPUT );
  pinMode( latchPin, OUTPUT );
  pinMode(5, OUTPUT );
  pinMode(7, OUTPUT );
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, cou);
  shiftOut(dataPin, clockPin, MSBFIRST, a);
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

  // Heure plus
  bthp = bthn;
  bthn = !digitalRead(5);

  if (bthn == 1 && bthp == 0) {
    decalage = decalage + 3600000;
    aff = 3;
    affdb = millis();
  }

  if (decalage > 172800000) {
    decalage = decalage - 86400000 ;
  }

  // Minutes plus

  btmp = btmn;
  btmn = !digitalRead(7);

  if (btmn == 1 && btmp == 0) {
    aff = 4;
    affdb = millis();
    decalage = decalage - sn;
    if (mn >= 59) {
      decalage = decalage - 3540000;
    } else {
      decalage = decalage + 60000;
    }
  }


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

  // Affichage reglage des heures
  if (aff == 3) {
    com = hn;
  }

  // Affichage reglage des minutes
  if (aff == 4) {
    com = mn;
  }


  chu = com % 10;
  chd = com / 10;


  // affichage collone
  if (a >= 2) {
    a = 1;
  } else {
    a = a++;
  }

  if (a == 1) {
    t = chd;
  } else {
    t = chu;
  }



  //Convertion du nombre en code pour affichage

  if (t == 0) {
    cou = 63;
  }

  if (t == 1) {
    cou = 6;
  }

  if (t == 2) {
    cou = 91;
  }

  if (t == 3) {
    cou = 79;
  }

  if (t == 4) {
    cou = 102;
  }

  if (t == 5) {
    cou = 109;
  }

  if (t == 6) {
    cou = 125;
  }

  if (t == 7) {
    cou = 7;
  }

  if (t == 8) {
    cou = 127;
  }


  if (t == 9) {
    cou = 111;
  }

  // Clignotement du point indicateur de l'unité
  if (a == 2) {
    afftpb = millis() % 1000;

    if ( aff == 0) {
      cl = afftpb % 1000;
      if (cl < 500) {
        bitWrite(cou, 7, 1);
      }
    }

    if ( aff == 1) {
      cl = afftpb % 333;
      if (cl < 166) {
        bitWrite(cou, 7, 1);
      }
    }

    if ( aff == 2) {
      cl = afftpb % 166;
      if (cl < 83) {
        bitWrite(cou, 7, 1);
      }
    }
  }
  // envoi du code a l'affichage

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, cou);
  shiftOut(dataPin, clockPin, MSBFIRST, a);
  digitalWrite(latchPin, HIGH);

}
