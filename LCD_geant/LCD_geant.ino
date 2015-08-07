#include <GDt6963c.h>

void setup()
{
      GLCD_Initalize(); // Initalize LCD
      GLCD_ClearText(); // Clear text area
      GLCD_ClearCG(); // Clear character generator area
      GLCD_ClearGraphic(); // Clear graphic area
      GLCD_TextGoTo(0,0);// set text coordinates
      GLCD_WriteString("Hello world !!! "); // write text
} 

void loop() {
      delay(1000);
      GLCD_TextGoTo(3,3);// set text coordinates
      GLCD_WriteString("AaAaAaA"); // write text

      delay(1000);
      GLCD_TextGoTo(3,3);// set text coordinates
      GLCD_WriteString("BbBbBbBb"); // write text
}
