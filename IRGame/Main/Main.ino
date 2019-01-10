//#include "Tasks.h"
//#include "ParentTaskClass.h"
#include "data/IRCommunication.h"
#include <Arduino.h>

int data[11];
int dataPoint = 0;
int dataBit;
boolean changedBit;

IRCommunicatie *ircommunicatie = new IRCommunicatie(frequency56kHz, false);
ISR(TIMER2_OVF_vect){
	ircommunicatie->counterPlusOneSending();
	ircommunicatie->counterPlusOneReceiving();
	if (ircommunicatie->getAllowedToSend())
	{
		ircommunicatie->encodetimeToLED(ircommunicatie->dummyTimes);
	}
	if (changedBit){
		changedBit = false;
		dataBit = (int) (((float) ircommunicatie->getCounterRECEIVING()) / compensatingValue);
		ircommunicatie->setCountersRECEIVINGToZero();
		if (dataBit < 50){
			//do nothing
		}else if (dataBit < 150){
			data[dataPoint] = 0;
		}else if (dataBit < 250){
			data[dataPoint] = 1;
		}else if (dataBit < 350){
			// parity
			data[dataPoint] = 2;
		}else if (dataBit < 450){
			//startbit
			dataPoint = 0;
			data[dataPoint] = 3;
		}else if (dataBit < 550){
			//stopbit
			data[dataPoint] = 4;
			for(int i = 0; i < 3; i++) {
				Serial.println(data[i]);
			}
		}else{
			//do nothing
		}
		dataPoint++;
	}
}

ISR(PCINT2_vect){
	sei();
	Serial.println( (int) (((float) ircommunicatie->getCounterRECEIVING()) / compensatingValue));
	//Serial.println(ircommunicatie->getCounterRECEIVING());
	changedBit = true;
}


int main(void){
	
	Serial.begin(9600);
	ircommunicatie->setHzfrequency();
	
	while(1){
	}
	Serial.end();
}
