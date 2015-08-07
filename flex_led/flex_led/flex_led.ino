void setup(){
  Serial.begin(9600); // On lance la liaison serie pour recuperer les infos
  pinMode(3,OUTPUT); // Place la broche D3 en sortie
}

void loop(){
  int flex = analogRead(0); // Lecture sur le port A0
  Serial.print(flex); // Ecrit la valeur de la flexation
  Serial.println(); // Saut à la ligne
  delay(500); // Attend 500 ms
  
  if ((flex <300) || (flex > 450)) 
    digitalWrite(3,1); //Allume la led
  else
    digitalWrite(3,0); // Eteind la led
}
