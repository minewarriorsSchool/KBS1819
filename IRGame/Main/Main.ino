//#include "Tasks.h"
//#include "ParentTaskClass.h"
#include "data/IRCommunication.h"
#include <Arduino.h>

IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz, false);
ISR(TIMER2_OVF_vect){
	ircommunicatie->counterPlusOne();
	if (ircommunicatie->getAllowedToSend())
	{
		ircommunicatie->encodetimeToLED(ircommunicatie->dummyTimes);
	}
}

ISR(PCINT2_vect){
	Serial.println(ircommunicatie->getCounterRECEIVING());
	ircommunicatie->setCountersRECEIVINGToZero();
}


int main(void){
	
	Serial.begin(9600);
	
	ircommunicatie->setHzfrequency();
	
	while(1){
	}
	Serial.end();
}
