// 
// 
// 

#include "Blokken.h"

Blokken::Blokken(){
	
}

Blokken::drawblock(int x0, int w, int h){
	screen.fillRect(x0, stap, w, h, rectcolor);
	screen.fillRect(x0, stap - 3, w, 3, BACKGROUND);	
}

TCNT1 = timer1_counter;   // preload timer
TCCR1B |= (1 << CS12);    // 256 prescaler
TIMSK1 |= (1 << TOIE1);