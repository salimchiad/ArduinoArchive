#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 8);
byte test;
int i;
byte A, B, C, D, E, F, G, H, I, J, K, L, M, N;
int command = 0;       // This is the command char, in ascii form, sent from the serial port
const unsigned char code[10] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 123};

void setup()
{
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  A = 1;
  B = 2;
  C = 3;
  D = 4;
  E = 5;
  F = 6;
  G = 7;
  H = 8;
}


void loop()
{
  lc.setRow(0, 0, code[A]);
  lc.setRow(0, 1, code[B]);
  lc.setRow(0, 2, code[C]);
  lc.setRow(0, 3, code[D]);
  lc.setRow(0, 4, code[E]);
  lc.setRow(0, 5, code[F]);
  lc.setRow(0, 6, code[G]);
  lc.setRow(0, 7, code[H]);
}
