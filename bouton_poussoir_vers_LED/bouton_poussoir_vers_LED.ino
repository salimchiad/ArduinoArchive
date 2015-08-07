int led = 13; // la LED est branchée sur la broche 13
int etat_bouton = 2; // le bouton est branché sur le broche 2
int bouton; // variable qui accueillera l'etat du bouton


void setup() {

  pinMode(etat_bouton, INPUT); // Reglage de la broche du bouton en tant qu'entrée
  pinMode(led, OUTPUT); // Reglage de la broche qui acceille la LED en tant que sortie
}

void loop() {

  bouton = digitalRead(etat_bouton);

  if (bouton == HIGH) { // Compare la valeur bouton a la valeur HIGH pour voir si elles sont égales
    digitalWrite(led, HIGH); // Si la valeur et vérifiée (donc on appui) alors on exécute ce qui se situe dans le bloc dans les prèmieres accolades { ... }
  } else {
    digitalWrite(led, LOW); // Sinon on excecute ce qui se situe dans le bloc qui se situe juste apres le mot "else"
  }

}
