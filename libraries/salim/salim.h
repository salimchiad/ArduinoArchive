#ifndef SALIM_H
#define SALIM_H

#include "Arduino.h"

class SALIM{
  public:
    SALIM(int ledPin);
	bool getState();
    void on();
	void off();
	void toggle();
	void blink(unsigned int time, byte times=1);
	void setValue(byte val);
	void fadeIn(unsigned int time);
	void fadeOut(unsigned int time);
  private:
	bool status;
	int pin;
};

extern SALIM DEBUG_LED;

#endif

/*
|| @changelog
|| | 1.1 2009-05-07 - Alexander Brevig : Added blink(uint,byte), requested by: Josiah Ritchie - josiah@josiahritchie.com
|| | 1.1 2009-04-07 - Alexander Brevig : Altered API
|| | 1.0 2009-04-17 - Alexander Brevig : Initial Release
|| #
*/
