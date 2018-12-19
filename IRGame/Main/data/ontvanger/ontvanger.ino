

#include <Arduino.h>

int COUNTER = 0;
boolean changebits = false;


// OVERFLOWS
int overflowBit0 = 55;
int overflowBit1 = 110;

ISR(TIMER2_OVF_vect){
	if(changebits){
		if(COUNTER != 0){
			if (COUNTER <= overflowBit0){
				Serial.println("bit0");
				}else if (COUNTER <= overflowBit1){
				Serial.println("bit1");
				}else{
				Serial.println("ERROR");
			}
		}
		COUNTER = 0;
		}else{
		if (COUNTER < 110){
			COUNTER++;
		}
	}
	changebits = false;
}

ISR(PCINT_T0_V){
	changebits = true;
}


//int LEDpin = 7;
//int ARDINpin = 3;
int IRpin = 5;

int main(){

	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);    //set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);                 //set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);
	sei();                                // enable global interrupts
	OCR2A = 35;
	OCR2B = 17;
	
	
	Serial.begin(9600);
	
	DDRD |= (1<<DDD7);
	DDRD &= ~(1 << DDD3) | (1 << IRpin);
	

	
	//PORTD |= (1 << LEDpin); // Pin n goes high
	
	while(1)
	{
		
		//		if (PIND & (1<< IRpin)){
		//			Serial.write("1");
		//			PORTD |= (1 << LEDpin); // Pin n goes high
		//		}
		//		else{
		//			Serial.write("0");
		//			PORTD &= ~(1 << LEDpin); // Pin n goes low
		//		}
		
		
	}
	
	
	return 0;
}