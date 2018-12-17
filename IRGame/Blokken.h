// Blokken.h

#ifndef _BLOKKEN_h
#define _BLOKKEN_h
#define BACKGROUND	0x0000
#define rectcolor GREY

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
	public:
		Blokken();
	
		int tijd;
		int stap, oudestap;
		int y0;
		int timer1_counter;
		
		void drawblock(int x0, int w, int h);

#endif

