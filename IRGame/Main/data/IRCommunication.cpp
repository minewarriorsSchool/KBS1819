//
//
//

#include "IRCommunication.h"

IRCommunicatie::IRCommunicatie(uint8_t frequency, boolean testMode){
	while(frequency!=frequency56kHz  && frequency!=frequency38kHz){}	//Making sure that no other frequency is allowed besides required frequencies4
	
	if(frequency == frequency38kHz){									//Setting the duty-cycle to 50%; and frequency to 38.000 Hz
		OCR2A = OCR2AWaarde38kHz;
		if (testMode == true)
		{
			OCR2B = OCR2AWaarde38kHz;
		} else OCR2B = OCR2BWaarde38kHz;
	}
	
	if(frequency == frequency56kHz){									//Setting the duty-cycle to 50%; and frequency to 56.000 Hz
		OCR2A = OCR2AWaarde56kHz;
		if(testMode == true){
			OCR2B = OCR2AWaarde56kHz;
		}else OCR2B = OCR2BWaarde56kHz;
	}
}

void IRCommunicatie::setHzfrequency(){
	PORTB |= (1<<PORTB5);												//Test LED PORT to high pin 13
	DDRB |= (1<<DDB5);													//Test LED to output pin 13
	PORTD |= (1<<PORTD3);												//PWM PIN 3 infra rood to high
	DDRD |= (1<<DDD3);													//PWM PIN 3 infra rood to output
	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);		//set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);									//set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);
	sei();																// enable global interrupts
}

void IRCommunicatie::changeCounter(boolean pinchange){					//on pinchange reset counter else keep counting overflows
	if(pinchange == true){												//if there is a pinchange reset the counter
		setCounterToZero()												//resetting counter
		PORTB ^= (1<<PORTB5);											//blink a LED to see the pinchange
		} else if(getCounter() == OverFlowCounterStopBit){					//if overflow is equal to stop-bit, stop adding overflows to counter and wait till pinchange to reset counter
		
		}else{															//else add 1 overflow to overflowcounter
		counter++														//up counter with 1
	}
}

int IRCommunicatie::getCounter(){
	return counter;
}

void IRCommunicatie::setCounterToZero(){
	counter = 0;
}

void IRCommunicatie::dataToSend(){
	/*Starting of with sending a startbit to make start data transfer
	startbit consists of an overflow counter around 40 overflows long*/
	if(stateOfprotocol == 0){
		if(statusSendingStartBit == false){											//If not busy with sending start bit, make change in pin to start sending startbit
			PORTD ^= (1<<PORTD3);													//Make pinchange in pin 3 PWM pin
			statusSendingStartBit = true;											//Set status to true because sending startbit												
		}
		if(getCounter() == OverFlowCounterStartBit){								// if overflow counter reached the time of the startbit set status startbit to completed and move to next step
			statusSendingStartBit = false;											// startbit time is reached so set the status of sending to false
			stateOfprotocol++;														// startbit has been reached so move te next step in the protocol
		}
	}
}