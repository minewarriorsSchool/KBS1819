// IRCommunication.h

#ifndef _IRCOMMUNICATION_h
#define _IRCOMMUNICATION_h
#define OCR2AWaarde56kHz 35 // 55.52kHz
#define OCR2BWaarde56kHz 17 // 50% 55.52kHz
#define OCR2AWaarde38kHz 52 // 37,71kHz
#define OCR2BWaarde38kHz 26 // 50.75% 37,71kHz
#define frequency56kHz 56	// definition of 56 kHz
#define frequency38kHz 38	// definition of 38 kHz
#define OverFlowCounterBit0 20
#define OverFlowCounterBit1 30
#define OverFlowCounterStartBit 50
#define OverFlowCounterStopBit 60
#define OverFlowCounterParity 40

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"s
#endif

class IRCommunicatie{
	public:
	IRCommunicatie(uint8_t frequency, boolean testMode);
	void setHzfrequency();
	void changeCounter(boolean pinchange);
	int getCounter();
	void setCounterToZero();
	void enablePWM();
	void disablePWM();
	void dataToSend();
	void setSendData(boolean Yes_No);
	void receivingDataFromSerial();
	void settingDataToSend(uint8_t Byte);
	
	protected:
	private:
	int counter = 0, stateOfprotocol = 0, DataInOverflows[8];
	boolean statusSendingBit = false, sendData = false;
	uint8_t Data;
	
};


#endif

