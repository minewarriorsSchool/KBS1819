// IRCommunication.h

#ifndef _IRCOMMUNICATION_h
#define _IRCOMMUNICATION_h
#define OCR2AWaarde56kHz 35 // 55.52kHz
#define OCR2BWaarde56kHz 17 // 50% 55.52kHz
#define OCR2AWaarde38kHz 52 // 37,71kHz
#define OCR2BWaarde38kHz 26 // 50.75% 37,71kHz
#define frequency56kHz 56	// definition of 56 kHz
#define frequency38kHz 38	// definition of 38 kHz
//#define OverFlowCounter56HzBit0 500
//#define OverFlowCounter56HzBit1 1000
//#define OverFlowCounter56HzStartBit 2000
//#define OverFlowCounter56HzStopBit 2500
//#define OverFlowCounter56HzParityBit 1500
//#define OverFlowCounter38HzBit0 100
//#define OverFlowCounter38HzBit1 200
//#define OverFlowCounter38HzStartBit 400
//#define OverFlowCounter38HzStopBit 500
//#define OverFlowCounter38HzParityBit 300
#define compensatingValue 1.473684
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
	
	void setBitOverFlows(uint8_t frequency);
	
	//+Variabelen
	int dummyTimes[11];
	protected:
	private:
	
	//-Variabelen
	int counterSending = 0, counterReceiving = 0, bitCounter = 0, dummyVariable[LengthArrayBits] = {1,0,1,0,1,0,1,0,1,0,1}, decodedTimes[LengthArrayBits],
	counterDataBit = 0,
	OverFlowCounterBit0 = 500,
	OverFlowCounterBit1 = 1000,
	OverFlowCounterStartBit = 2000,
	OverFlowCounterStopBit = 2500,
	OverFlowCounterParityBit = 1500;
	boolean nextBit = true, startBitActive = true, stopBitActive = false, parityBitActive = false, allowedToSend, isDataAvailable = false;
};


#endif

