int led = 11; // la LED est branchée sur la broche 11
int etat_bouton = 9; // le bouton est branché sur le broche 9
int bouton; // variable qui accueillera l'etat du bouton
int i;

void setup() {
  pinMode(etat_bouton, INPUT); // Reglage de la broche du bouton en tant qu'entrée
  pinMode(led, OUTPUT); // Reglage de la broche qui acceille la LED en tant que sortie
}

void loop() {

  bouton = digitalRead(etat_bouton);

  if (bouton == HIGH) { // Compare la valeur bouton a la valeur HIGH pour voir si elles sont égales

    for (i=0;i<255;i=i+1){
    analogWrite(led, i);
    delay(1);
    }
    
    digitalWrite(led, HIGH);
    
   delay(2000);
   
    for (i=255;i>0;i=i-1){
    analogWrite(led, i);
    delay(4);
    }

  } else {
    digitalWrite(led, LOW); // Sinon on excecute ce qui se situe dans le bloc qui se situe juste apres le mot "else"
  }
}
