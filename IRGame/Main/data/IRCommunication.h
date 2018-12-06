/*
* IRCommunication.h
*
* Created: 05/12/2018 18:32:37
* Author: jelle
*/


#ifndef __IRCOMMUNICATION_H__
#define __IRCOMMUNICATION_H__

#include <Arduino.h>


class IRCommunication
{
	//variables
	public:
	protected:
	private:
	
	//Declaring registers here
	/*
	going for fast PWM mode
	16.000.000/56.000 = 285,71428571428571428571428571429 --> so prescale it down with 8
	16.000.000/56.000/8 close to 36 (35,714285714285714285714285714286)
	16.000.000/38.000/8 close to 53 (52,631578947368421052631578947368)
	*/
	#define OC56KHZ 36;
	#define OC38KHZ 53;

	/*
	Clear OC2B on Compare match, set OC2B at bottom, (non-inverting mode)
	Fast PWM mode TOP: 0xFF, Update of OCRx at Bottom, TOV flag set on MAX
	*/
	TCCR2A |= (1<<COM2B1)| (1<<WGM21) | (1<<WGM20);
	
	//pre-scaling 8 --> for correct prescaling
	TCCR2B |= (1<<CS21);
	
	/*
	OCR2B = 53 if sender is 38 KHZ
	OCR2b = 36 if sender is 56 KHZ
	*/

	//End declaring registers

	//functions
	public:
	IRCommunication();//Constructor
	~IRCommunication();//Destructor
	initTask(uint8_t frequencyInKHz); //Controls which frequencies are accepted
	frequencyInteruptTask(uint8_t frequency); //Controls which frequency is being received
	protected:
	private:
	IRCommunication( const IRCommunication &c );
	IRCommunication& operator=( const IRCommunication &c );
}; //IRCommunication

#endif
