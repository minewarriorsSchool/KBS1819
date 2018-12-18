#include "Poppetje.h"
#include "data/IRCommunication.h"
#include <Arduino.h>

IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz, true);

ISR(TIMER2_OVF_vect){
	ircommunicatie->dataToSend();
	Serial.println(ircommunicatie->getCounter());
}


int main(void){
	PORTB |= (1<<PORTB5);
	DDRB |= (1<<DDB5);
	
	ircommunicatie->setHzfrequency();
	
	Serial.begin(9600);
	while(1){
		ircommunicatie->setSendData(true);
	}
	Serial.end();
}

