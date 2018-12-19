//
//
//

#include "IRCommunication.h"

//Constructor
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
//End-Constructor

//Functies
void IRCommunicatie::setHzfrequency(){
	PORTB |= (1<<PORTB5);												//if LED 13 is high, IR can not send
	DDRB |= (1<<DDB5);													//Test LED to output pin 13
	PORTD |= (1<<PORTD3);												//PWM PIN 3 infra rood to high
	DDRD |= (1<<DDD3);													//PWM PIN 3 infra rood to output
	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);		//set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);									//set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);

	//Debug encoding BIT to DATA
	encodingToTime(dummyVariable);
	//end-debug
	
	sei();																// enable global interrupts
}

void IRCommunicatie::counterPlusOne(){
	counter++;
}

void IRCommunicatie::encodingToTime(int *Byte){
	for (int i=0; i<=7; i++)
	{
		if(Byte[i] == 1){
			dummyTimes[i] = OverFlowCounterBit1;
			} else if(Byte[i] == 0){
			dummyTimes[i] = OverFlowCounterBit0;
		}else Serial.println("Setting BIT to TIME error");
	}
	for (int i=0; i<=7; i++)
	{
		Serial.println((dummyTimes[i] ));
	}
}

void IRCommunicatie::encodetimeToLED(int *Times){

	if(nextBit){
		PORTB ^= (1<<PORTB5);
		setCounterToZero();
		nextBit = false;
	}
	
	if(getCounter() == Times[bitCounter]){
		nextBit = true;
		Serial.print("Counter: ");
		Serial.println(getCounter());
		bitCounter++;
		if(bitCounter==7) bitCounter = 0;
	}
}

//End-Functies

//Getters
int IRCommunicatie::getCounter(){
	return counter;
}
//End-Getters

//Setters
void IRCommunicatie::setCounterToZero(){
	counter = 0;
}
//End-Setters