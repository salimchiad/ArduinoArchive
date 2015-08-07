#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x20, 16, 2); // set the LCD address of the first lcd to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x21, 16, 2); // set the LCD address of the second lcd to 0x21 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd3(0x22, 16, 2); // set the LCD address of the first lcd to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd4(0x23, 16, 2); // set the LCD address of the second lcd to 0x21 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd5(0x24, 16, 2); // set the LCD address of the first lcd to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd6(0x25, 16, 2); // set the LCD address of the second lcd to 0x21 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd7(0x26, 16, 2); // set the LCD address of the first lcd to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd8(0x27, 16, 2); // set the LCD address of the second lcd to 0x21 for a 16 chars and 2 line display

void setup()
{
  lcd1.init();                      // initialize the first lcd
  lcd2.init();                      // initialize the second lcd
  lcd3.init();                      // initialize the first lcd
  lcd4.init();                      // initialize the second lcd
  lcd5.init();                      // initialize the first lcd
  lcd6.init();                      // initialize the second lcd
  lcd7.init();                      // initialize the first lcd
  lcd8.init();                      // initialize the second lcd

  // Print a message on the first LCD.
  
  lcd1.print("Hello, #1 world!");

  // Print a message on the second LCD.
  
  lcd2.print("Hello, #2 world!");
  
    // Print a message on the first LCD.
  
  lcd3.print("Hello, #3 world!");

  // Print a message on the second LCD.
  
  lcd4.print("Hello, #4 world!");
  
    // Print a message on the first LCD.
  
  lcd5.print("Hello, #5 world!");

  // Print a message on the second LCD.
  
  lcd6.print("Hello, #6 world!");
    // Print a message on the first LCD.
  
  
  lcd7.print("Hello, #7 world!");

  // Print a message on the second LCD.
 
  lcd8.print("Hello, #8 world!");

}

void loop()
{
}
