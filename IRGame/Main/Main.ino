
int main(void){
	PORTD |= (1<<PORTD3);
	DDRD |= (1<<DDD3);
	TCCR2A |= (1<<COM2A0)| (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare B
	TCCR2B |= (1 << CS21) | (1 << WGM22);								//set clock prescaler to 8 and Fast PWM
	//OCR2A = 35;														// 55.52kHz
	//OCR2B = 17;														// 50% 55.52kHz
	OCR2A = 52; // 37,71kHz
	OCR2B = 26; // 50.75% 37,71kHz
}
