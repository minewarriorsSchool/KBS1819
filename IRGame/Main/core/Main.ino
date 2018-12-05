#include <Arduino.h>

//Declaring registers here
/* 
going for fast PWM mode
16.000.000/56.000/8 close to 36 (35,714285714285714285714285714286)
16.000.000/38.000/8 close to 53 (52,631578947368421052631578947368)
*/

/*
Clear OC2B on Compare match, set OC2B at bottom, (non-inverting mode)
Fast PWM mode TOP: 0xFF, Update of OCRx at Bottom, TOV flag set on MAX
*/
TCCR2A |= (1<<COM2B1)| (1<<WGM21) | (1<<WGM20);
//pre-scaling 8
TCCR2B |= (1<<CS20);
/* OCR2B = 53 if sender is 38 KHZ
   OCR2b = 36 if sender is 56 KHZ
*/

//End declaring registers

int main(void){
	
}