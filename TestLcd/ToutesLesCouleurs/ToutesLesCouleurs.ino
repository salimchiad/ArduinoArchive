// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
#include <math.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A6 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

unsigned int x, y, w, h;
long color;

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
void setup(void) {
  Serial.begin(115200);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); 
  Serial.print(tft.width()); 
  Serial.print("x"); 
  Serial.println(tft.height());
  w = tft.width();
  h = tft.height();

  tft.reset();

  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } 
  else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } 
  else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } 
  else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } 
  else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } 
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);
  tft.setRotation(0);
}


void loop(void) {
  tft.fillScreen(WHITE);

  for (y = 0; y < h; y++){
    for (x = 0; x < w; x++){
      color = (x + (240 * y));
      tft.drawPixel(x, y, color);
    }
  }
  tft.fillScreen(BLACK);
  tft.drawLine(0, 0, w, h, RED);
  tft.drawLine(0, h, w, 0, BLUE);
  tft.drawLine(0, h/2, w, h/2, YELLOW);
  tft.drawLine(0, h/4, w, h/4, CYAN);
  tft.drawLine(0, 3*h/4, w, 3*h/4, GREEN);
  tft.drawLine(w/2, 0, w/2, h, MAGENTA);
  tft.drawLine(w/4, 0, w/4, h, RED);
  tft.drawLine(3*w/4, 0, 3*w/4, h, BLUE);
  tft.drawLine(100, 100, 110, 310, BLUE);
  for (y = 0; y < h; y++){
    tft.drawPixel(120+((120.0*(cos((y/51.0))))), y, CYAN);
  }
  delay(10000);



}








