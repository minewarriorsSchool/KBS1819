//
//
//

#include "IRCommunication.h"

IRCommunicatie::IRCommunicatie(uint8_t frequency){
	while(frequency!=frequency56kHz  && frequency!=frequency38kHz){}	//Making sure that no other frequency is allowed besides required frequencies
	if(frequency == frequency38kHz){									//Setting the duty-cycle to 50%; and frequency to 38.000 Hz
		OCR2A = OCR2AWaarde38kHz;
		OCR2B = OCR2BWaarde38kHz;
	}
	if(frequency == frequency56kHz){									//Setting the duty-cycle to 50%; and frequency to 56.000 Hz
		OCR2A = OCR2AWaarde56kHz;
		OCR2B = OCR2BWaarde56kHz;
	}
}

void IRCommunicatie::setHzfrequency(){
	PORTD |= (1<<PORTD3);												
	DDRD |= (1<<DDD3);
	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);		//set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);									//set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);
	sei();																// enable global interrupts
}

void IRCommunicatie::changeCounter(boolean pinchange){
	if(pinchange == true){
		counter = 0;
	} else counter++;
}

int IRCommunicatie::getCounter(){
	return counter;
}

void IRCommunicatie::setCounterToZero(){
	counter = 0;
}