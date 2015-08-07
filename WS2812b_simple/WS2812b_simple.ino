#include <Adafruit_NeoPixel.h>
#define PIN 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
byte R, G, B;
char command = 0;
byte i;

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  if (Serial.available()) {      // Look for char in serial que and process if found
    command = Serial.read();
    if (command == 82 || command == 114){   // commande R ou r
      R = Serial.parseInt();
      Serial.print("R");
      Serial.println(R);
    }

    else if (command == 71 || command == 103){   // commande G ou g
      G = Serial.parseInt();
      Serial.print("G");
      Serial.println(G);
    }

    else if (command == 66 || command == 98){   // commande B ou b
      B = Serial.parseInt();
      Serial.print("B");
      Serial.println(B);
    }
  } 
  else {
    for(i = 0; i < 60; i++){ //definition couleur du fond
      strip.setPixelColor(i, R/4, G/4, B/4);
      strip.show();
    }
  }
}
