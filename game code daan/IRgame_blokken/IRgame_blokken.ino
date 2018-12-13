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

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);

int tijd;
int stap, oudestap;
int rectcolor = GREEN;
int y0;

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
	for(;;) {
		drawblock(100, 30, 30);
	}
}

void drawblock(int x0, int w, int h) {
	screen.fillRect(x0, stap, w, h, rectcolor);
	screen.fillRect(x0, stap - 3, w, 3, BACKGROUND);
}

ISR(TIMER1_OVF_vect)				//de interrupt die idere 1000ste van een seconde word aangeroepen
{
	TCNT1 = timer1_counter;			// de timer begint weer opnieuw
	tijd++;							// de tijd word met 1 verhoogt
	//Serial.println(tijd);
	if (tijd > 10) {
		stap++;
		tijd = 0;
	}
}
