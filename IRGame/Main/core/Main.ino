#include <Arduino.h>

//Declaring registers here
/* going for fast PWM mode so Fpwm = Fclk I:O / N * 256 --> 56khz  && 32khz wanted
*/

TCCR2A = (1<<WGM21) | (1<<WGM20) // Fast PWM mode TOP: 0xFF, Update of OCRx at Bottom, TOV flag set on MAX
TCCR2B = (1<<CS22) | (1<<CS20) // prescaling 128

//End declaring registers

int main(void){
	
}