//
//
//

#include "IRCommunication.h"

//Constructor
IRCommunicatie::IRCommunicatie(uint8_t frequency, boolean testMode){
	while(frequency!=frequency56kHz  && frequency!=frequency38kHz){}		//Making sure that no other frequency is allowed besides required frequencies4
	
	if(frequency == frequency38kHz){										//Setting the duty-cycle to 50%; and frequency to 38.000 Hz
		OCR2A = OCR2AWaarde38kHz;
		if (testMode == true)
		{
			OCR2B = OCR2AWaarde38kHz;
		} else OCR2B = OCR2BWaarde38kHz;
	}
	
	if(frequency == frequency56kHz){										//Setting the duty-cycle to 50%; and frequency to 56.000 Hz
		OCR2A = OCR2AWaarde56kHz;
		if(testMode == true){
			OCR2B = OCR2AWaarde56kHz;
		}else OCR2B = OCR2BWaarde56kHz;
	}
}
//End-Constructor

//Functies
void IRCommunicatie::setHzfrequency(){
	PORTB |= (1<<PORTB5);													//if LED 13 is high, IR can not send
	DDRB |= (1<<DDB5);														//Test LED to output pin 13
	PORTD |= (1<<PORTD3);													//PWM PIN 3 infra rood to high
	DDRD |= (1<<DDD3);														//PWM PIN 3 infra rood to output
	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);			//set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);										//set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);

	//Debug encoding BIT to DATA
	encodingToTime(dummyVariable);
	//end-debug
	
	sei();																	// enable global interrupts
}

void IRCommunicatie::counterPlusOne(){
	counter++;
}

/*This function encodes an array of bits to time frames*/
void IRCommunicatie::encodingToTime(int *Byte){
	for (int i=0; i<=7; i++)												//loops through all 8 bits of data array
	{
		if(Byte[i] == 1){													//if the bit is 1
			dummyTimes[i] = OverFlowCounterBit1;							//turn the 1 into required define time for bit1 and put the value in another array
			} else if(Byte[i] == 0){										//as above, but than for the bit value 0
			dummyTimes[i] = OverFlowCounterBit0;
		}else Serial.println("Setting BIT to TIME error");					//debug for if something went wrong in converting bits to time
	} setAllowedToSend(true);
	
	//start debug
	//for (int i=0; i<=7; i++)
	//{
	//	Serial.println((dummyTimes[i] ));
	//}
	//end debug
}

/*encodeTimeToLED encodes the times to LED switched due to multiplexing*/
void IRCommunicatie::encodetimeToLED(int *Times){
	if(startBitActive && !stopBitActive && !parityBitActive){				//Check for sending start bit or not
		nextBitQuestionMark();												//If boolean nextbit is true it will make a flip on portb5
		if(getCounter() == OverFlowCounterStartBit){
			nextBit = true;
			//Serial.print("Counter StartTime: ");							// debug
			//Serial.println(getCounter());									// debug
			startBitActive = false;
		}
		}else if (!startBitActive && stopBitActive && !parityBitActive){	//Check for sending stop bit or not
		nextBitQuestionMark();
		if(getCounter() == OverFlowCounterStopBit){
			nextBit = true;
			//Serial.print("Counter StopTime: ");							// debug
			//Serial.println(getCounter());									// debug
			stopBitActive = false;
			startBitActive = true;
			//PORTB ^=(1<<PORTB5);
			//Serial.println("StopBit has been send +  parity");
			setAllowedToSend(false);
		}
		} else if(!startBitActive && !stopBitActive && parityBitActive){
		nextBitQuestionMark();
		if(getCounter() == OverFlowParityBit){
			nextBit = true;
			//Serial.print("Counter ParityTime: ");							// debug
			//Serial.println(getCounter());									// debug
			parityBitActive = false;
			stopBitActive = true;
		}
		} else if(!startBitActive && !stopBitActive && !parityBitActive){
		nextBitQuestionMark();
		if(getCounter() == Times[bitCounter]){								//Checks if the right amount of ticks for the required bit has been reached
			nextBit = true;													//If reached it is time to switch states again in previous statement to count ticks for the next bit
			//Serial.print("Counter DataTime: ");							// debug
			//Serial.println(getCounter());									// debug
			bitCounter++;													//get next bit in array
			if(bitCounter==8){												//if the bitcounter is at the end of the data arrary, set back to [0] for the next data array
				parityBitActive = true;
				bitCounter = 0;
			}
		}
	} else Serial.println("Something went wrong!!!!");
	
	void IRCommunicatie::nextBitQuestionMark(){
		if(nextBit){														//nextbit is by natural true to start data transfer
			PORTB ^= (1<<PORTB5);											//switched state of LED pin 13 to let PWM pin 3 be able to send or not send --> multiplexing
			setCounterToZero();												//set counter to 0 to start counting the right amount of ticks equal to the bit required
			nextBit = false;												//making sure the LED does not change state while tick counting
		}
	}
}
//End-Functies

//Getters
int IRCommunicatie::getCounter(){
	return counter;
}

boolean IRCommunicatie::getAllowedToSend(){
	return allowedToSend;
}
//End-Getters

//Setters
void IRCommunicatie::setCounterToZero(){
	counter = 0;
}

void IRCommunicatie::setAllowedToSend(boolean YES_NO){
	allowedToSend = YES_NO;
}
//End-Setters