// IRCommunication.h

#ifndef _IRCOMMUNICATION_h
#define _IRCOMMUNICATION_h
#define OCR2AWaarde56kHz 35 // 55.52kHz
#define OCR2BWaarde56kHz 17 // 50% 55.52kHz
#define OCR2AWaarde38kHz 52 // 37,71kHz
#define OCR2BWaarde38kHz 26 // 50.75% 37,71kHz
#define frequency56kHz 56	// definition of 56 kHz
#define frequency38kHz 38	// definition of 38 kHz
#define OverFlowCounterBit0 100
#define OverFlowCounterBit1 200
#define OverFlowCounterStartBit 300
#define OverFlowCounterStopBit 500
#define OverFlowParityBit 400
#define compensatingValue38kHz 1,4736842105263157894736842105263
#define compensatingValue56kHz 0,67857142857142857142857142857143
#define LengthArrayBits 11

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"s
#endif

class IRCommunicatie{
	public:
	//Constructor
	IRCommunicatie(uint8_t frequency, boolean testMode);
	
	//Functies
	void counterPlusOne();
	void setHzfrequency();
	void encodingToTime(int *Byte);
	void encodetimeToLED(int *Times);
	void nextBitQuestionMark();
	
	//Getters
	int getCounter();
	boolean getAllowedToSend();
	//Setters
	void setCounterToZero();
	void setAllowedToSend(boolean YES_NO);
	
	//+Variabelen
	int dummyTimes[11];
	protected:
	private:
	
	//-Variabelen
	long compensatingValue;
	int counterSending = 0, bitCounter = 0, dummyVariable[11] = {1,0,1,0,1,0,1,0,1,0,1};
	boolean nextBit = true, startBitActive = true, stopBitActive = false, parityBitActive = false, allowedToSend;
};


#endif

