#include "Poppetje.h"
#include "data/IRCommunication.h"
#include <Arduino.h>

volatile uint16_t counter = 0;
ISR(TIMER2_OVF_vect){
	counter++;
	
	if (counter == 1)
	{
		change();
		counter = 0;
	}
	Serial.println(counter);
}


int main(void){
	PORTB |= (1<<PORTB5);
	DDRB |= (1<<DDB5);
	
	Serial.begin(9600);
	
	IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz);
	ircommunicatie->setHzfrequency();
	
	while(1){
	}
	Serial.end();
}

void change(){
	PORTB ^= (1<<PORTB5);
	PORTD ^= (1<<PORTD3);
	Serial.println("Changefunction");
}
