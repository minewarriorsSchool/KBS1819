

#include <Arduino.h>

int COUNTER = 0;
boolean changebits = false;
int frequentie = 38;
int data[8];
int dataPointer = 0;

// OVERFLOWS
int overflowBit0 = 100;
int overflowBit1 = 220;

void checkCounter(){
  if(COUNTER != 0){
//    if (COUNTER <= overflowBit0){
//      Serial.println("bit0");
//      }else if (COUNTER <= overflowBit1){
//      Serial.println("bit1");
//      }else{
//      Serial.println("ERROR");
//    }
      data[dataPointer] = COUNTER;
      dataPointer++;
//      if(dataPointer == 8){
//        dataPointer = 0;
//        for(int i = 0; i < 8; i++)
//          {
//            Serial.print(i, ": ");
//            Serial.println(data[i]);
//          }
//      }
      Serial.println(COUNTER);    //DIT IS EEN TEST
  }
}

ISR(TIMER2_OVF_vect){
  if(COUNTER < 70){
    COUNTER++;
  }
}

ISR(PCINT2_vect){
	changebits = true;
  checkCounter();
  COUNTER = 0;
}


//int LEDpin = 7;
//int ARDINpin = 3;
int IRpin = 5;

int main(){

	TCCR2A |= (1<<COM2A0)| (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);    //set compare B
	TCCR2B |= (1<<CS21) | (1<<WGM22);                 //set clock pre-scaler to 8 and Fast PWM
	TIMSK2 |= (1<<TOIE2);
	sei();                                // enable global interrupts
	OCR2A = 52;
	OCR2B = 26;

  PORTD |= (1 << PORTD5); //pullup
	PCICR |= (1 << PCIE2);    /* set pin-change interrupt for D pins */
  PCMSK2 |= (1 << PCINT21); /* set mask to look for PCINT18 / PD2 */
  
	Serial.begin(9600);
	
//	DDRD |= (1<<DDD7);
//	DDRD &= ~(1 << DDD3) | (1 << IRpin);
	
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
