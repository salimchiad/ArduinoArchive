:

#include "SALIM.h"

SALIM::SALIM(int ledPin){
	pin=ledPin;
	status=LOW;
	pinMode(pin,OUTPUT);
}

bool SALIM::getState(){ return status; }

void SALIM::on(void){
	digitalWrite(pin,HIGH);
	status=true;
}

void SALIM::off(void){
	digitalWrite(pin,LOW);
	status=false;
}

void SALIM::toggle(void){
	status ? off() : on();
}

void SALIM::blink(unsigned int time, byte times){
	for (byte i=0; i<times; i++){
		toggle();
		delay(time/2);
		toggle();
		delay(time/2);
	}
}

//assume PWM
void SALIM::setValue(byte val){
	analogWrite(pin,val);
	val==0 ? status=false : status=true;
}

//assume PWM
void SALIM::fadeIn(unsigned int time){
	for (byte value = 0 ; value < 255; value+=5){
		analogWrite(pin, value);
		delay(time/(255/5));
	}
	on();
}

//assume PWM
void SALIM::fadeOut(unsigned int time){
	for (byte value = 255; value >0; value-=5){
		analogWrite(pin, value);
		delay(time/(255/5));
	}
	off();
}

extern SALIM DEBUG_SALIM(13);

/*
|| @changelog
|| | 2009-04-07 - Alexander Brevig : Altered API
|| | 2009-04-17 - Alexander Brevig : Initial Release
|| #
*/
