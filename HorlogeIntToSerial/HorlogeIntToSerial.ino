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
int z = 0;



void setup() {
  Serial.begin(57600);

}



void loop() {
  ms = millis() + decalage;
  s = ms / 1000;
  sn = s % 60;
  m = s / 60;
  mn = m % 60;
  h = m / 60;
  hn = h % 24;

  if (Serial.available()) {
    z = Serial.read();
  } else {
    z = 0;
  }


  // Heure plus
  if (z == 72) {
    decalage = decalage + 3600000;
    if (decalage > 172800000) {
      decalage = decalage - 86400000 ;
    }
  }

  // Minutes plus


  if (z == 77) {
    decalage = decalage - sn;
    if (mn >= 59) {
      decalage = decalage - 3540000;
    } else {
      decalage = decalage + 60000;
    }
  }

  Serial.print(hn);
  Serial.print(":");
  Serial.print(mn);
  Serial.print(":");
  Serial.println(sn);
  delay(10);

}
