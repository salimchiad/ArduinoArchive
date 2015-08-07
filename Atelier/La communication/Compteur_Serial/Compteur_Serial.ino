int i = 0;
char message[20] = "Nombre actuel : ";

void setup() {
  Serial.begin(9600);
}

void loop() {

  for (i = 0; i < 20; i++)
  {
    Serial.print(message);
    Serial.println(i);
  }
  
  for (i = 0; i < 18; i++)
  {
    Serial.print(message[i], DEC);
    Serial.print(" ");
    }
  
  delay (10000);

}
