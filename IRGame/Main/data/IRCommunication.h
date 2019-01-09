// IRCommunication.h

#ifndef _IRCOMMUNICATION_h
#define _IRCOMMUNICATION_h
#define OCR2AWaarde56kHz 35 // 55.52kHz
#define OCR2BWaarde56kHz 17 // 50% 55.52kHz
#define OCR2AWaarde38kHz 52 // 37,71kHz
#define OCR2BWaarde38kHz 26 // 50.75% 37,71kHz
#define frequency56kHz 56	// definition of 56 kHz
#define frequency38kHz 38	// definition of 38 kHz
#define Devines01OverFlowCounter0 500
#define Devines01OverFlowCounterBit1 1000
#define Devines01OverFlowCounterStartBit 2000
#define Devines01OverFlowCounterStopBit 2500
#define Devines01OverFlowParityBit 1500
#define Devines01compensatingValue38kHz 1,47
#define Devines01compensatingValue56kHz 0,67
#define Devines02OverFlowCounter0 100
#define Devines02OverFlowCounterBit1 200
#define Devines02OverFlowCounterStartBit 400
#define Devines02OverFlowCounterStopBit 500
#define Devines02OverFlowParityBit 300
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
	void counterDataBitPlus_Reset();
	void counterPlusOneSending();
	void counterPlusOneReceiving();
	void setHzfrequency();
	void encodingToTime(int *Byte);
	void encodetimeToLED(int *Times);
	void nextBitQuestionMark();
	void decodingTimes();
	
	//Getters
	int getCounterSENDING();
	int getCounterRECEIVING();
	boolean getAllowedToSend();
	//Setters
	void setCountersSENDINGToZero();
	void setCountersRECEIVINGToZero();
	void setAllowedToSend(boolean YES_NO);
	void setIsDataAvailable(boolean YES_NO);
	
	//+Variabelen
	int dummyTimes[11];
	protected:
	private:
	
	//-Variabelen
	int counterSending = 0, counterReceiving = 0, bitCounter = 0, dummyVariable[LengthArrayBits] = {1,0,1,0,1,0,1,0,1,0,1}, decodedTimes[LengthArrayBits],
		 counterDataBit = 0;
	boolean nextBit = true, startBitActive = true, stopBitActive = false, parityBitActive = false, allowedToSend, isDataAvailable = false;
};


#endif

