#include <GDt6963c.h>

char ligne1[26], ligne2[22], ligne3[20], ligne4;
unsigned int secondes, psecondes, in;

void setup()
{
  GLCD_Initalize(); // Initalize LCD
  GLCD_ClearText(); // Clear text area
  GLCD_ClearCG(); // Clear character generator area
  GLCD_ClearGraphic(); // Clear graphic area
  GLCD_TextGoTo(0,0);// set text coordinates
  GLCD_WriteString("  Station meteo version 0.1"); // write text
  Serial.begin(57600);
} 

void loop() {


  if (Serial.available() > 0) {
    // read the incoming byte:
    for (int i = 0; i < 26; i++){
      in = Serial.read();
      ligne1[i] = in;
    }
    for (int i = 0; i < 22; i++){
      in = Serial.read();
      ligne2[i] = in;
    }
    for (int i = 0; i < 20; i++){
      in = Serial.read();
      ligne3[i] = in;
    }
  }

    GLCD_TextGoTo(0,3);// set text coordinates
    GLCD_WriteString(ligne1); // write text
    delay(10);
    GLCD_TextGoTo(0,4);// set text coordinates
    GLCD_WriteString(ligne2); // write text
    delay(10);
    GLCD_TextGoTo(0,5);// set text coordinates
    GLCD_WriteString(ligne3); // write text
    delay(1000);
    Serial.println("r");

}





