#include <Arduino.h>

//Declaring registers here
/* going for fast PWM mode so Fpwm = Fclk I:O / N * 256 --> 56khz  && 32khz wanted
*/

TCCR2A = (1<<0) | (1<<0) // Fast PWM mode

//End declaring registers

int main(void){
	
}