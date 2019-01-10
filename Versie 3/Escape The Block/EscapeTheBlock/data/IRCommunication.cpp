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
	
	setBitOverFlows(frequency);
}
//End-Constructor

//Functies
void IRCommunicatie::setHzfrequency(){
	
	//Start Of Send Setup
	PORTB |= (1<<PORTB5);													//if LED 13 is high, IR can not send
	DDRB |= (1<<DDB5);														//Test LED to output pin 13
	PORTD |= (1<<PORTD3);													//PWM PIN 3 infra rood to high
	DDRD |= (1<<DDD3);														//PWM PIN 3 infra rood to output
	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);			//set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);										//set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);
	//End of Send Setup
	
	//Start of Receive Setup
	PORTD |= (1 << PORTD5);													//pullup digital pin 5
	PCICR |= (1 << PCIE2);													/* set pin-change interrupt for D register */
	PCMSK2 |= (1 << PCINT21);												/* set mask to look for PCINT18, PD2, digital pin 5*/
	//End Of Receive Setup
	
	

	//Debug encoding BIT to DATA
	encodingToTime(dummyVariable);
	//end-debug
	
	sei();																	// enable global interrupts
}


//START OF SENDING INFRARED CODE
void IRCommunicatie::counterPlusOneSending(){
	counterSending++;
}

void IRCommunicatie::counterPlusOneReceiving(){
	counterReceiving++;
}

/*This function encodes an array of bits to time frames*/
void IRCommunicatie::encodingToTime(int *Byte){
	for (int i=0; i<LengthArrayBits; i++)												//loops through all LENGTHARRAYBITS of data array
	{
		if(Byte[i] == 1){													//if the bit is 1
				dummyTimes[i] = OverFlowCounterBit1;							//turn the 1 into required define time for bit1 and put the value in another array
			//Serial.println(dummyTimes[i]);									//debug
			} else if(Byte[i] == 0){										//as above, but than for the bit value 0
			dummyTimes[i] = OverFlowCounterBit0;
			//Serial.println(dummyTimes[i]);									//debug
		}else Serial.println("Setting BIT to TIME error");					//debug for if something went wrong in converting bits to time
	} setAllowedToSend(true);
	
	//start debug
	/*for (int i=0; i<=7; i++)
	{
	Serial.println((dummyTimes[i] ));
	}*/
	//end debug
}

/*encodeTimeToLED encodes the times to LED switched due to multiplexing*/
void IRCommunicatie::encodetimeToLED(int *Times){
	if(startBitActive && !stopBitActive && !parityBitActive){				//Check for sending start bit or not
		nextBitQuestionMark();												//If boolean nextbit is true it will make a flip on portb5
		if(getCounterSENDING() == OverFlowCounterStartBit){
			nextBit = true;
			/*Serial.print("Counter StartTime: ");							// debug
			Serial.println(getCounter());		*/							// debug
			startBitActive = false;
		}
		}else if (!startBitActive && stopBitActive && !parityBitActive){	//Check for sending stop bit or not
		nextBitQuestionMark();
		if(getCounterSENDING() == OverFlowCounterStopBit){
			nextBit = true;
			/*Serial.print("Counter StopTime: ");							// debug
			Serial.println(getCounter());	*/								// debug
			stopBitActive = false;
			startBitActive = true;
			//PORTB ^=(1<<PORTB5);
			//Serial.println("StopBit has been send +  parity");
			setAllowedToSend(true);
			//setAllowedToSend(false);	//og
		}
		} else if(!startBitActive && !stopBitActive && parityBitActive){
		nextBitQuestionMark();
		if(getCounterSENDING() == OverFlowCounterParityBit){
			nextBit = true;
			/*Serial.print("Counter ParityTime: ");							// debug
			Serial.println(getCounter());*/									// debug
			parityBitActive = false;
			stopBitActive = true;
		}
		} else if(!startBitActive && !stopBitActive && !parityBitActive){
		nextBitQuestionMark();
		if(getCounterSENDING() == Times[bitCounter]){								//Checks if the right amount of ticks for the required bit has been reached
			nextBit = true;													//If reached it is time to switch states again in previous statement to count ticks for the next bit
			/*Serial.print("Counter DataTime: ");							// debug
			Serial.println(getCounter());*/									// debug
			bitCounter++;													//get next bit in array
			if(bitCounter==LengthArrayBits){												//if the bitcounter is at the end of the data arrary, set back to [0] for the next data array
				parityBitActive = true;
				bitCounter = 0;
			}
		}
	} else Serial.println("Something went wrong!!!!");
	
}

void IRCommunicatie::nextBitQuestionMark(){
	if(nextBit){														//nextbit is by natural true to start data transfer
		PORTB ^= (1<<PORTB5);											//switched state of LED pin 13 to let PWM pin 3 be able to send or not send --> multiplexing
		DDRD ^= (1<<DDD3); //test without MP
		setCountersSENDINGToZero();												//set counterSending to 0 to start counting the right amount of ticks equal to the bit required
		nextBit = false;												//making sure the LED does not change state while tick counting
	}
}
//End-Functies

//Getters
int IRCommunicatie::getCounterSENDING(){
	return counterSending;
}

boolean IRCommunicatie::getAllowedToSend(){
	return allowedToSend;
}
//End-Getters

//Setters
void IRCommunicatie::setBitOverFlows(uint8_t frequency){
	if (frequency == frequency56kHz){
		OverFlowCounterBit0 = 500;
		OverFlowCounterBit1 = 1000;
		OverFlowCounterStartBit = 2000;
		OverFlowCounterStopBit = 2500;
		OverFlowCounterParityBit = 1500;
	}else if(frequency == frequency38kHz){
		OverFlowCounterBit0 = 100;
		OverFlowCounterBit1 = 200;
		OverFlowCounterStartBit = 400;
		OverFlowCounterStopBit = 500;
		OverFlowCounterParityBit = 300;
	}
}


void IRCommunicatie::setCountersSENDINGToZero(){
	counterSending = 0;
}

void IRCommunicatie::setAllowedToSend(boolean YES_NO){
	allowedToSend = YES_NO;
}
//End-Setters

//END OF SENDING INFRARED CODE

//START OF RECEIVING INFRARED CODE
//Getters
int IRCommunicatie::getCounterRECEIVING(){
	return counterReceiving;
}
//End-Getters
//Setters
void IRCommunicatie::setCountersRECEIVINGToZero(){
	counterReceiving = 0;
}

//void IRCommunicatie::decodingTimes(int time){
	//if(counterDataBit == 0){
	//}
//}
//
//void IRCommunicatie::setIsDataAvailable(boolean YES_NO){
	//isDataAvailable = YES_NO;
//}
//End-Setters
//END OF RECEIVING INFRARED CODE