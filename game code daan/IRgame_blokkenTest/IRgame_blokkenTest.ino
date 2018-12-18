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
#define rectcolor GREY
#define MAXARRAY 100
#define MAXLEVENS 15		//de hoeveelheid levens die je hebt (er passen er 12 op het scherm)

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);

int speler1kleur = BLUE;
int speler2kleur = RED;

int pointerPos = 1;
int oudpointerPos = 2;
int scherm = 1, oudescherm = 1;

int tijd;				//de tijd die de timer ieder ms met 1 verhoogd
int stap1, stap2;		// de y coordinaten van blok 1 en 2
int blok2;				// bij 1 mag blok2 vallen, bij 0 niet
int snelheid = 5;		//snelheid waarmee de blokken vallen (2 = heel snel, 4 = langzaam)
int blokweg = 10;		// de grootte van het zwarte vierkant wat de blokken achtervolgt om het spoor weg te halen
int checkcollision = 0;	// bij 0 is er geen collision, bij 1 wel
int collisiontijd;		// de score die de speler had toen de collision plaatsvond
int invincibility = 1;	// de tijd dat je onoverwinnelijk bent nadat je bent geraakt
int geraakt, score, extrascore;

#define BACKGROUND	0x0000

int locaties [100], groottes [100];
int randomlocatie, randomgrootte, randomafstand = 180;
int nummer1 = 1, nummer2 = 50;
int timer1_counter;
int check;

int joyx;			//de x-coordinaten van het charcter
int joyy = 275;		//de y-coordinaten van het charcter, 275 is de beginlocatie van het onderste charachter


int charachterx = 115;	//de x locatie van het charchter, als de nunchuck naar rechts/links word geduwd komt er iedere loop 1 bij/af
//115 is de beginlocatie waaruit de charachter wordt getekend


int main (){
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
	screen.fillScreen(BACKGROUND);
	
	nunchuck_setpowerpins();
	nunchuck_init();			//nunchuck initialiseren
	Serial.begin(9600);
	
	seed();
	
	for(;;){
		nunchuck_get_data();	//nunchuck data ophalen
		joyy = nunchuck_joyy();	//y data van de joystick ophalen
		joyx = nunchuck_joyx();	//x data van de joystick ophalen
		
		/*		UITLEZEN
		Serial.print("scherm: ");
		Serial.print(scherm);
		Serial.print(", c button: ");
		Serial.print(nunchuck_cbutton());
		Serial.print(", z button: ");
		Serial.print(nunchuck_zbutton());
		Serial.print(", joyy: ");
		Serial.print(joyy);
		Serial.print(", joyx: ");
		Serial.println(joyx);
		*/
		
		while (joyy == 0 && joyx == 0 && nunchuck_cbutton() == 1 && nunchuck_zbutton() == 1) {	//check of de nunchuck is aangesloten
			check = 0;
			screen.setCursor(50, 40);		// print dat de nunchuck niet is aangesloten
			screen.setTextColor(WHITE);
			screen.setTextSize(3);
			screen.println("PLEASE");
			screen.setCursor(50, 70);
			screen.println("CONNECT");
			screen.setCursor(50, 100);
			screen.println("NUNCHUCK");
			screen.setCursor(50, 130);
			screen.println("AND");
			screen.setCursor(50, 160);
			screen.println("RESET");
		}
		if (check == 0) {		// het scherm weer eenmaal zwart maken om de tekst "please connect nunchuck" weg te halen
			check = 1;
			screen.fillScreen(BACKGROUND);
		}
		
		clearScreen();			// als het scherm is veranderd alles eerst zwart maken
		
		
		
		if(scherm == 1){		//scherm 1 is het homescreen
			drawHomescreen();
			drawPointer();
		}
		
		if(scherm == 2){		//scherm 2 is het spel
			start();
		}
		
		if(scherm == 3){		//scherm 3 is de highscores
			highscore();
		}
		
		if(scherm == 4){		//scherm 4 is de controls
			controls();
		}
		
		if (scherm == 5){		//scherm 5 is het gameover scherm
			gameover();
		}
	}
}

void clearScreen (){
	if (scherm != oudescherm) {			//het scherm word geleegd als je van scherm veranderd
		screen.fillScreen(BACKGROUND);	
		if(scherm == 1) {				//het spel is terug gegaan naar het hoofdmenu dus alles word gereset naar de beginwaardes
			stap1 = 0;
			stap2 = 0;
			geraakt = 0;
			score = 0;
			blok2 = 0;
			nummer1 = 1;
			nummer2 = MAXARRAY / 2;
			snelheid = 5;
			blokweg = 10;
			checkcollision = 0;
			collisiontijd = 0;
			seed();					// nieuwe seed berekenen
		}
		oudescherm = scherm;
	}
}

void drawHomescreen(){				//het homescreen
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);  screen.setTextSize(3);
	screen.println("Escape the");
	screen.setCursor(77, 70);
	screen.setTextColor(WHITE);  screen.setTextSize(3);
	screen.println("BLOCK");
	screen.setCursor(90, 130);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("Start");
	screen.setCursor(65, 150);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("Highscore");
	screen.setCursor(71, 171);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("Controls");
	
	
	speler1kleur = BLUE;
	speler2kleur = RED;
	score = 0;
	
	drawcharHomescreen(50, 275, speler2kleur);		//teken twee players ter decoratie op het homescreen
	drawcharHomescreen(180, 235, speler1kleur);
}


void drawcharHomescreen(int x, int y, int color){	//een player tekenen
	screen.fillCircle(x, y, 10, color);
	screen.fillRoundRect(x - 16, y - 5, 32, 10, 5, color);
}


void drawPointer(){			//het tekenen  van de aanwijzer
	
	if (oudpointerPos != pointerPos){	//als de positie van de pointer is veranderd haal je de pointer weg van de vorigie positie
		if (oudpointerPos == 1){
			screen.fillTriangle(45, 131, 45, 141, 55, 136, BACKGROUND);
		}
		
		if (oudpointerPos == 2){
			screen.fillTriangle(45, 151, 45, 161, 55, 156, BACKGROUND);
		}
		
		if (oudpointerPos == 3){
			screen.fillTriangle(45, 171, 45, 181, 55, 176, BACKGROUND);
		}
		
	}
	
	if (pointerPos == 1){
		screen.fillTriangle(45, 131, 45, 141, 55, 136, GREEN); //teken een pointer op positie 1
		if(nunchuck_zbutton() == 1){
			scherm = 2;		//als de pointer op positie 1 is en er word op z gedrukt, ga naar scherm 2 (start)
		}
	}
	
	if (pointerPos == 2){
		screen.fillTriangle(45, 151, 45, 161, 55, 156, GREEN);	//teken een pointer op positie 2
		
		if(nunchuck_zbutton() == 1){
			scherm = 3;		//als de pointer op positie 2 is en er word op z gedrukt, ga naar scherm 3 (Highscores)
		}
	}
	
	if (pointerPos == 3){
		screen.fillTriangle(45, 171, 45, 181, 55, 176, GREEN);	//teken een pointer op positie 3
		
		if(nunchuck_zbutton() == 1){
			scherm = 4;		//als de pointer op positie 3 is en er word op z gedrukt, ga naar scherm 2 (controls)
		}
	}
	
	oudpointerPos = pointerPos;
	
	_delay_ms(50);
	

	if (joyy > 160 && pointerPos > 1) {
		pointerPos--;
	} else if (joyy < 80 && pointerPos < 3) {
		pointerPos++;
	}
	
}

void start(){

	drawcharacter(joyx, 275, speler1kleur); 
	
	drawblock(locaties[nummer1], groottes[nummer1]);
	drawblock2(locaties[nummer2], groottes[nummer2]);
	
	collision();
	header();
	
	if (MAXLEVENS - geraakt <= 0) {
		scherm = 5;
	}
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}

void gameover() {
	screen.setCursor(40, 70);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("GAME OVER");
	screen.setCursor(70, 160);
	screen.setTextSize(2);
	screen.print("score: ");
	screen.println(score);
	screen.setCursor(130, 300);
	screen.setTextSize(1);
	screen.println("press c to go back");
	
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}

void highscore(){
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("HIGHSCORES");
	
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}


void controls(){
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("CONTROLS");
	screen.setCursor(30, 80);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("Move the joystick");
	screen.setCursor(30, 100);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("to go left and");
	screen.setCursor(30, 120);

	drawcharacter(joyx, 160, RED);

	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("right");
	screen.setCursor(30, 190);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("Press C to go");
	screen.setCursor(30, 210);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("back to the");
	screen.setCursor(30, 230);
	screen.setTextColor(WHITE);  screen.setTextSize(2);
	screen.println("homescreen");


	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}


void drawblock(int x, int grootte) {
	if (nummer1 != 49) {
		screen.fillRect(x, stap1 - grootte, grootte, grootte, rectcolor);
		screen.fillRect(x, stap1 - grootte - blokweg, grootte, blokweg, BACKGROUND);
		
		if (stap1 > 330 + grootte && stap2 > randomafstand){
			nummer1++;
			screen.fillRect(200, 0, 50, 10, BACKGROUND);		//oude score weghalen om plaats te maken voor de nieuwe score
			stap1 = 0;
		}
	}
}

void drawblock2(int x, int grootte) {
	if (stap1 >= 160) {
		blok2 = 1;		//als het aller eerste blok1 weg is mag blok2 ook beginnen
	}
	
	if (blok2 == 1) {
		screen.fillRect(x, stap2, grootte, grootte, CYAN);			//blok2 tekenen
		screen.fillRect(x, stap2 - blokweg, grootte, blokweg, BACKGROUND);
		
		if (stap2 > (330 + grootte) && stap1 > randomafstand){	// als het vorige blok2 uit het scherm is en blok1 is ver genoeg
			nummer2++;											// volgende blok2
			screen.fillRect(200, 0, 50, 10, BACKGROUND);		// oude score weghalen om plaats te maken voor de nieuwe score
			stap2 = 0;											//stappenteller reseten
			randomafstand = rand() % 20 + 180;					// nieuwe random afstand uitrekenen die tussen de blokken moet zitten
		}
	}
	
	if(nummer2 == MAXARRAY) {
		nummer1 = 0;
		nummer2 = MAXARRAY / 2;
		extrascore = extrascore + MAXARRAY;
		seed();
	}
	
	if (score >= 25) {
		snelheid = 4;				// de blokken vallen sloom
		blokweg = 15;				// de grootte van het zwarte blok wat de blokke achtervolgt om het spoor weg te halen
		invincibility = 2;			// als je geraakt word ben je voor de tijd van '1' score punt ononverwinnelijk
		if (score >= 50) {
			snelheid = 3;			// de blokken vallen iets sneller
			blokweg = 17;
			invincibility = 3;
			if (score >= 75) {
				snelheid = 2;		// de blokken vallen heel snel
				blokweg = 20;
				invincibility = 4;
				if (score >= 125) {
					snelheid = 1;
					blokweg = 25;
					invincibility = 5;
				}
			}
		}
	}
}

void seed (){
	for(int i = 0; i <= MAXARRAY; i++) {
		groottes[i] = rand() % 25 + 25;
		locaties[i] = rand() % (240 - groottes[i]);
	}
}

ISR(TIMER1_OVF_vect)				//de interrupt die idere 1000ste van een seconde word aangeroepen
{
	TCNT1 = timer1_counter;			// de timer begint weer opnieuw
	tijd++;							// de tijd word met 1 verhoogt
	
	if (tijd > snelheid) {
		stap1++;
		if (blok2 == 1){
			stap2++;
		}
		tijd = 0;
	}
}

void header() {
	screen.setCursor(0, 0);
	screen.setTextColor(WHITE);  
	screen.setTextSize(1);
	screen.print("levens");
	
	for(int lev = 40; (lev < (MAXLEVENS - geraakt) * 10 + 40) && lev < 160; lev = lev + 10){	//het teken van alle levens
		screen.fillCircle(lev, 4, 3, RED);
	}
	
	screen.setCursor(160, 0);
	screen.print("score: ");
	score = nummer1 + (nummer2 - MAXARRAY / 2) + extrascore;
	screen.print(score);
	
}

void collision (){
	if (checkcollision == 0){
		if(locaties[nummer1] + groottes[nummer1] >= charachterx - 15 && locaties[nummer1] <= charachterx - 15 && (stap1 >= 265 && stap1 <= 285)) {	// controleren of blok1 de linker kant van de speler heeft geraakt
			checkcollision = 1;
			collisiontijd = score;	//de tijd van de collsion opslaan zodat de speler voor een korte tijd invinceble is
			geraakt++;				// de speler is één keer vaker geraakt en verliest dus ook één leven
		}
		
		if(locaties[nummer1] + groottes[nummer1] >= charachterx + 15 && locaties[nummer1] <= charachterx + 15 && (stap1 >= 265 && stap1 <= 285)) {	// controleren of blok1 de rechter kant van de speler heeft geraakt
			checkcollision = 1;
			collisiontijd = score;
			geraakt++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx - 15 && locaties[nummer2] <= charachterx - 15 && (stap2 >= 265 && stap2 <= 285)) {	// controleren of blok2 de linker kant van de speler heeft geraakt
			checkcollision = 1;
			collisiontijd = score;
			geraakt++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx + 15 && locaties[nummer2] <= charachterx + 15 && (stap2 >= 265 && stap2 <= 285)) {	// controleren of blok2 de rechter kant van de speler heeft geraakt
			checkcollision = 1;
			collisiontijd = score;
			geraakt++;
		}
	}
	
	if (checkcollision == 1) {	//er is een botsing geweest
		screen.fillRect(0, 0, 160, 10, BACKGROUND);
		speler1kleur = WHITE;
		if ((collisiontijd + invincibility) < score) {
			checkcollision = 0;
			speler1kleur = BLUE;
		}
	}
	/*		UITLEZEN
	Serial.print("score: ");
	Serial.print(score);
	Serial.print("   collisiontijd: ");
	Serial.print(collisiontijd);
	Serial.print("   invincibility: ");
	Serial.println(invincibility);
	*/
}



void drawcharacter(int x, int y, int Color){
	
	
	if (x > 140 && charachterx < 225) {			//de x coordinaten van de charachter mag niet hoger worden dan 225, anders gaat hij uit het scherm
		charachterx = charachterx + 4;								//als de nunchuck naar rechts is geduwd, gaat het charachter ook naar rechts
		} else if (x < 90 && charachterx > 15) {	//de y coordinaten van de charachter mag niet lager worden dan 15, anders gaat hij uit het scherm
		charachterx = charachterx - 4;								//als de nunchuck naar links is geduwd, gaat het charachter ook naar links
	}
	
	//teken het charachter
	screen.fillCircle(charachterx, y, 10, Color);						//hoofd
	screen.fillRoundRect(charachterx - 16, y - 5, 32, 10, 5, Color);	//schouders
	
	
	//een lijn om het charachter heen tekenen in dezelfde kleur als de achtergrond
	// B = boven, O = onder, L = links, R = rechts, hoofd is de middelste circel en de schouders zijn de uitsteksels aan de linker en rechter kant
	screen.drawLine(charachterx - 4, y - 11, charachterx + 4, y - 11, BACKGROUND);									// B hoofd
	screen.fillTriangle(charachterx - 4, y - 11, charachterx - 9, y - 6, charachterx - 9, y - 11, BACKGROUND);		// LB hoofd
	screen.fillRect(charachterx - 14, y - 9, 6, 4, BACKGROUND);														// L schouder B
	screen.fillTriangle(charachterx - 13, y - 6, charachterx - 16, y - 2, charachterx - 16, y - 6, BACKGROUND);		// L shouder LB
	screen.fillRect(charachterx - 18, y - 6, 4, 13, BACKGROUND);													// L schouder L
	screen.fillTriangle(charachterx - 13, y + 6, charachterx - 16, y + 2, charachterx - 16, y + 6, BACKGROUND);		// L schouder LO
	screen.fillRect(charachterx - 14, y + 5, 6, 4, BACKGROUND);														// L schouder O
	screen.fillTriangle(charachterx - 4, y + 11, charachterx - 9, y + 6, charachterx - 9, y + 11, BACKGROUND);		// LO hoofd
	screen.drawLine(charachterx - 4, y + 11, charachterx + 4, y + 11, BACKGROUND);									// O hoofd
	screen.fillTriangle(charachterx + 9, y + 6, charachterx + 4, y + 11, charachterx + 9, y + 11, BACKGROUND);		// RO hoofd
	screen.fillRect(charachterx + 8, y + 5, 6, 4, BACKGROUND);														// R schouder O
	screen.fillTriangle(charachterx + 13, y + 6, charachterx + 16, y + 2, charachterx + 16, y + 6, BACKGROUND);		// R schouder RO
	screen.fillRect(charachterx + 16, y - 6, 4, 13, BACKGROUND);													// R schouder R
	screen.fillTriangle(charachterx + 13, y - 6, charachterx + 16, y - 2, charachterx + 16, y - 6, BACKGROUND);		// R shouder RB
	screen.fillRect(charachterx + 8, y - 9, 6, 4, BACKGROUND);														// R schouder B
	screen.fillTriangle(charachterx + 4, y - 11, charachterx + 9, y - 6, charachterx + 9, y - 11, BACKGROUND);		// RB hoofd
}




//________________________________

static uint8_t nunchuck_buf[6]; // array to store nunchuck data,


// Uses port C (analog in) pins as power & ground for Nunchuck

static void nunchuck_setpowerpins()

{

	#define pwrpin PC3

	#define gndpin PC2

	DDRC |= _BV(pwrpin) | _BV(gndpin);

	PORTC &=~ _BV(gndpin);

	PORTC |= _BV(pwrpin);

	delay(100); // wait for things to stabilize

}


// initialize the I2C system, join the I2C bus,

// and tell the nunchuck we're talking to it

void nunchuck_init()

{

	Wire.begin();  // join i2c bus as master

	Wire.beginTransmission(0x52); // transmit to device 0x52

	Wire.write(0x40);  // sends memory address

	Wire.write(0x00);  // sends sent a zero.

	Wire.endTransmission(); // stop transmitting

}


// Send a request for data to the nunchuck

// was "send_zero()"

void nunchuck_send_request()

{

	Wire.beginTransmission(0x52); // transmit to device 0x52

	Wire.write(0x00);  // sends one byte

	Wire.endTransmission(); // stop transmitting

}


// Receive data back from the nunchuck,

// returns 1 on successful read. returns 0 on failure

int nunchuck_get_data()

{

	int cnt=0;

	Wire.requestFrom (0x52, 6); // request data from nunchuck

	while (Wire.available ()) {

		// receive byte as an integer

		nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());

		cnt++;

	}

	nunchuck_send_request(); // send request for next data payload

	// If we recieved the 6 bytes, then go print them

	if (cnt >= 5) {

		return 1; // success

	}

	return 0; //failure

}


// Print the input data we have recieved

// accel data is 10 bits long

// so we read 8 bits, then we have to add

// on the last 2 bits. That is why I

// multiply them by 2 * 2

void nunchuck_print_data()

{

	static int i=0;

	int joy_x_axis = nunchuck_buf[0];

	int joy_y_axis = nunchuck_buf[1];

	int accel_x_axis = nunchuck_buf[2]; // * 2 * 2;

	int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;

	int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;


	int z_button = 0;

	int c_button = 0;


	// byte nunchuck_buf[5] contains bits for z and c buttons

	// it also contains the least significant bits for the accelerometer data

	// so we have to check each bit of byte outbuf[5]

	if ((nunchuck_buf[5] >> 0) & 1)

	z_button = 1;

	if ((nunchuck_buf[5] >> 1) & 1)

	c_button = 1;


	if ((nunchuck_buf[5] >> 2) & 1)

	accel_x_axis += 2;

	if ((nunchuck_buf[5] >> 3) & 1)

	accel_x_axis += 1;


	if ((nunchuck_buf[5] >> 4) & 1)

	accel_y_axis += 2;

	if ((nunchuck_buf[5] >> 5) & 1)

	accel_y_axis += 1;


	if ((nunchuck_buf[5] >> 6) & 1)

	accel_z_axis += 2;

	if ((nunchuck_buf[5] >> 7) & 1)

	accel_z_axis += 1;


	Serial.print(i,DEC);

	Serial.print("\t");


	Serial.print("joy:");

	Serial.print(joy_x_axis,DEC);

	Serial.print(",");

	Serial.print(joy_y_axis, DEC);

	Serial.print(" \t");


	Serial.print("acc:");

	Serial.print(accel_x_axis, DEC);

	Serial.print(",");

	Serial.print(accel_y_axis, DEC);

	Serial.print(",");

	Serial.print(accel_z_axis, DEC);

	Serial.print("\t");


	Serial.print("but:");

	Serial.print(z_button, DEC);

	Serial.print(",");

	Serial.print(c_button, DEC);


	Serial.print("\r\n"); // newline

	i++;

}


// Encode data to format that most wiimote drivers except

// only needed if you use one of the regular wiimote drivers

char nunchuk_decode_byte (char x)

{

	x = (x ^ 0x17) + 0x17;

	return x;

}


// returns zbutton state: 1=pressed, 0=notpressed

int nunchuck_zbutton()

{

	return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1; // voodoo

}


// returns zbutton state: 1=pressed, 0=notpressed

int nunchuck_cbutton()

{

	return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1; // voodoo

}


// returns value of x-axis joystick

int nunchuck_joyx()

{

	return nunchuck_buf[0];

}


// returns value of y-axis joystick

int nunchuck_joyy()

{

	return nunchuck_buf[1];

}


// returns value of x-axis accelerometer

int nunchuck_accelx()

{

	return nunchuck_buf[2]; // FIXME: this leaves out 2-bits of the data

}


// returns value of y-axis accelerometer

int nunchuck_accely()

{

	return nunchuck_buf[3]; // FIXME: this leaves out 2-bits of the data

}


// returns value of z-axis accelerometer

int nunchuck_accelz()

{

	return nunchuck_buf[4]; // FIXME: this leaves out 2-bits of the data

}
