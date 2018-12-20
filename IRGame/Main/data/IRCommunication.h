// IRCommunication.h

#ifndef _IRCOMMUNICATION_h
#define _IRCOMMUNICATION_h
#define OCR2AWaarde56kHz 35 // 55.52kHz
#define OCR2BWaarde56kHz 17 // 50% 55.52kHz
#define OCR2AWaarde38kHz 52 // 37,71kHz
#define OCR2BWaarde38kHz 26 // 50.75% 37,71kHz
#define frequency56kHz 56	// definition of 56 kHz
#define frequency38kHz 38	// definition of 38 kHz
#define OverFlowCounterBit0 10
#define OverFlowCounterBit1 20
#define OverFlowCounterStartBit 40
#define OverFlowCounterStopBit 50
#define OverFlowParityBit 30

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
	
	//Getters
	int getCounter();
	boolean getAllowedToSend();
	//Setters
	void setCounterToZero();
	void setAllowedToSend(boolean YES_NO);
	
	//+Variabelen
	int dummyTimes[8];
	protected:
	private:
	
	//-Variabelen
	int counter = 0, bitCounter = 0, dummyVariable[8] = {1,0,1,0,1,0,1,0};
	boolean nextBit = true, startBitActive = true, stopBitActive = false, parityBitActive = false, allowedToSend;
};


#endif

