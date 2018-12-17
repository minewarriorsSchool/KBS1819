// charachter.h

#ifndef _CHARACHTER_h
#define _CHARACHTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
	public:
		charachter();
	
		int joyx;			
		int joyy = 275;		
		int charachterx = 115;
		
		void drawcharacter(int x, int y, int Color);

#endif

