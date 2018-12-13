#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>    // Core graphics library

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD6BA

#define BACKGROUND	0x0000
#define rectcolor GREY


#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);

int tijd;
int stap, oudestap;

int y0;
int snelheid = 10;

int locaties [50], groottes [50]; 
int randomlocatie, randomgrootte;
int nummer = 0;

int timer1_counter;


int main(void) {
	
	noInterrupts();           // disable alle interrupts
	TCCR1A = 0;
	TCCR1B = 0;
	
	timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
	
	TCNT1 = timer1_counter;   // preload timer
	TCCR1B |= (1 << CS12);    // 256 prescaler
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();             // enable alle interrupts
	
	init();
	screen.begin();
	
	screen.fillScreen(BACKGROUND);	//achtergrond van het scherm word op zwart gezet
	
	
	Serial.begin(19200);
	
	seed();
	for(;;) {
		drawblock(locaties[nummer], groottes[nummer]);
		if (stap > 320){
			nummer++;
			stap = 0;
		}
		
	}
}

void drawblock(int x0, int grootte) {
	screen.fillRect(x0, stap, grootte, grootte, rectcolor);
	screen.fillRect(x0, stap - 10, grootte, 10, BACKGROUND);
}

void seed (){
	for(int i; i < 50; i++) {
		groottes[i] = rand() % 100 + 50;
		locaties[i] = rand() % 210;
	}
}

ISR(TIMER1_OVF_vect)				//de interrupt die idere 1000ste van een seconde word aangeroepen
{
	TCNT1 = timer1_counter;			// de timer begint weer opnieuw
	tijd++;							// de tijd word met 1 verhoogt
	
	if (tijd > 10) {
		stap++;
		//Serial.println(stap);
		tijd = 0;
	}
}


