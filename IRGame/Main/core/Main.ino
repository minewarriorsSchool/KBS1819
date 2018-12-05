#include <Arduino.h>

//Declaring registers here
/* going for fast PWM mode
16.000.000/56.000/8 close to 36 (35,714285714285714285714285714286)
16.000.000/38.000/8 close to 53 (52,631578947368421052631578947368)
*/

TCCR2A = (1<<WGM21) | (1<<WGM20) // Fast PWM mode TOP: 0xFF, Update of OCRx at Bottom, TOV flag set on MAX
TCCR2B = (1<<CS22) | (1<<CS20) // prescaling 128

//End declaring registers

int main(void){
	
}