#include <stdlib.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>
#define PIN 3

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ400);

int i;
int b = 20;
char* st;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(9600);
}

void loop() {
  i = 0;

  while (Serial.available())
  {
    st[i] = Serial.read();
  }
  st[i] = '\0';

  while (st[i])
  {
    if (st[i] == 's')
      i = setled((const char*)st, i);
    i++;
  }
}

int  setled(char *st, int s)
{
  int r;
  int g;
  int b;
  int i;

  i = 0;
  while (i < 14)
  {
    if (st[s + i] == 'r')
    {
      i++;
      r = atoi(st[s + i]);
    }
    else if (st[s + i] == 'g')
    {
      i++;
      g = atoi(st[s + i]);
    }
    else if (st[s + i] == 'b')
    {
      i++;
      b = atoi(st[s + i]);
    }
    else
    i++;
    strip.show();
  }
  return (s + i);
}
