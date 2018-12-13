#include "Poppetje.h"
#include "data/IRCommunication.h"
#include <Arduino.h>

volatile uint16_t counter = 0;
ISR(TIMER2_OVF_vect){
	uint8_t status = (PORTB & (1<<PORTB0));
	//Serial.println(counter);
	Serial.print(status);
	DDRD ^= (1<<DDD3);
}


int main(void){
	Serial.begin(9600);
	IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency38kHz);
	ircommunicatie->setHzfrequency();
	while(1){
	}
	Serial.end();
}
