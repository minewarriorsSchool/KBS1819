#include "data/IRCommunication.h"
#include <Arduino.h>

IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz, true);
uint8_t test;
ISR(TIMER2_OVF_vect){
}


int main(void){
	PORTB |= (1<<PORTB5);
	DDRB |= (1<<DDB5);
	
	ircommunicatie->setHzfrequency();
	
	Serial.begin(9600);
	while(1){
	}
	Serial.end();
}

