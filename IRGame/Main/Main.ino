#include "data/IRCommunication.h"
#include <Arduino.h>

IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz, false);
uint8_t test;
ISR(TIMER2_OVF_vect){
	ircommunicatie->counterPlusOne();
	if (ircommunicatie->getAllowedToSend())
	{
		ircommunicatie->encodetimeToLED(ircommunicatie->dummyTimes);
	}
}


int main(void){
	
	Serial.begin(9600);
	
	ircommunicatie->setHzfrequency();
	
	while(1){
	}
	Serial.end();
}

