/*
* IRCommunication.cpp
*
* Created: 05/12/2018 18:32:37
* Author: jelle
*/


#include "IRCommunication.h"

// default constructor
IRCommunication::IRCommunication()
{
} //IRCommunication

// default destructor
IRCommunication::~IRCommunication()
{
} //~IRCommunication

IRCommunication::init(uint8_t frequencyInKHz){
	while(frequencyInKHz != OC38KHZ || != OC56KHZ){ //Other frequencies are not allowed to continue
		break;
	}
}