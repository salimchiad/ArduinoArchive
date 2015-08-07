int leda = 13;
int ledb = 12;
int ledc = 11;
int ledd = 10;
int lede = 9;
int ledf = 8;
int ledg = 7;
int ledh = 6;



void setup() {                
  // initialiser le digital pin comme une sortie

  pinMode(leda, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledc, OUTPUT);
  pinMode(ledd, OUTPUT);
  pinMode(lede, OUTPUT);
  pinMode(ledf, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledh, OUTPUT);  
}

void loop() {



  //Compteur de 0 à 9 


  //************ 0 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme éteinte
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, LOW);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme allumée 

  delay(1000);              // attendre 1 sec

  //************ 1 *************
  digitalWrite(leda, LOW);   // fixe la led comme  éteinte
  digitalWrite(ledb, HIGH);   // fixe la led comme  éteinte
  digitalWrite(ledc, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledd, LOW);   // fixe la led comme éteinte
  digitalWrite(lede, LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, LOW);   // fixe la led comme éteinte
  digitalWrite(ledg, LOW);   // fixe la led comme allumé
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 2 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, LOW);   // fixe la led comme éteinte
  digitalWrite(ledd, HIGH);   // fixe la led comme allumé
  digitalWrite(lede, HIGH);   // fixe la led comme allumé
  digitalWrite(ledf, LOW);   // fixe la led comme allumé
  digitalWrite(ledg, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 3 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledd, HIGH);   // fixe la led comme allumé
  digitalWrite(lede, LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, LOW);   // fixe la led comme allumé
  digitalWrite(ledg, HIGH);   // fixe la led comme allumé
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //***** 4 *************
  digitalWrite(leda, LOW);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledc, HIGH);   // fixe la led comme allumé
  digitalWrite(ledd, LOW);   // fixe la led comme allumé
  digitalWrite(lede, LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledg, HIGH);   // fixe la led comme allumé
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);
  //************ 5 *************
  digitalWrite(leda, HIGH);   // fixe la led comme éteinte  
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledd, HIGH);   // fixe la led comme éteinte
  digitalWrite(lede, LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 6 *************
  digitalWrite(leda, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 7 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme éteinte
  digitalWrite(ledd, LOW);   // fixe la led comme éteinte
  digitalWrite(lede, LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, LOW);   // fixe la led comme éteinte
  digitalWrite(ledg, LOW);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 8 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  //************ 9 *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede,  LOW);   // fixe la led comme éteinte
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte  

  delay(1000);              // attendre 1 sec
  
  
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, LOW);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
  
  
    delay(1000);              // attendre 1 sec
  //************ B *************
  digitalWrite(leda, LOW);   // fixe la led comme allumée
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
  
    delay(1000);              // attendre 1 sec
  //************ C *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, LOW);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, LOW);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
  
  
    delay(1000);              // attendre 1 sec
  //************ D *************
  digitalWrite(leda, LOW);   // fixe la led comme allumée
  digitalWrite(ledb, HIGH);   // fixe la led comme allumée
  digitalWrite(ledc, HIGH);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, LOW);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
  
    delay(1000);              // attendre 1 sec
  //************ E *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, LOW);   // fixe la led comme allumée
  digitalWrite(ledd, HIGH);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
  
    delay(1000);              // attendre 1 sec
  //************ F *************
  digitalWrite(leda, HIGH);   // fixe la led comme allumée
  digitalWrite(ledb, LOW);   // fixe la led comme allumée
  digitalWrite(ledc, LOW);   // fixe la led comme allumée
  digitalWrite(ledd, LOW);   // fixe la led comme allumée
  digitalWrite(lede, HIGH);   // fixe la led comme allumée
  digitalWrite(ledf, HIGH);   // fixe la led comme allumée
  digitalWrite(ledg, HIGH);   // fixe la led comme allumée
  digitalWrite(ledh, LOW);   // fixe la led comme éteinte 
  
    delay(1000);
  
  

}

