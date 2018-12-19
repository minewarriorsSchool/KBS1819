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

int pointerPos = 1;
int oudpointerPos = 2;
int scherm = 1, oudescherm = 1;

int joyx;			//de x-coordinaten van het charcter
int joyy = 275;		//de y-coordinaten van het charcter, 275 is de beginlocatie van het onderste charachter


int charachterx = 115;	//de x locatie van het charchter, als de nunchuck naar rechts/links word geduwd komt er iedere loop 1 bij/af
//115 is de beginlocatie waaruit de charachter wordt getekend


int main (){
	init();
	screen.begin();
	screen.fillScreen(BACKGROUND);
	nunchuck_setpowerpins();
	nunchuck_init();
	Serial.begin(9600);
	
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
		Serial.println(nunchuck_zbutton());
		*/
		
		clearScreen();
		
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
	}
}

void clearScreen(){
	if (scherm != oudescherm) {			//het scherm wor eerst geleegd als je van scherm veranderd
		screen.fillScreen(BACKGROUND);	
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
	
	drawcharHomescreen(50, 275, RED);		//teken twee players ter decoratie op het homescreen
	drawcharHomescreen(180, 235, BLUE);
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
		oudpointerPos = pointerPos;
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
	
	
	_delay_ms(200);
	

	if (joyy > 140 && pointerPos > 1) {
		pointerPos--;
	} else if (joyy < 90 && pointerPos < 3) {
		pointerPos++;
	}
}

void start(){
	drawcharacter(joyx, 275, BLUE);
	
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
	
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}


//functie voor het teken van het charachter op de juiste locatie
void drawcharacter(int x, int y, int Color){
	
	
	if (x > 140 && charachterx < 225) {			//de x coordinaten van de charachter mag niet hoger worden dan 225, anders gaat hij uit het scherm
		charachterx++;								//als de nunchuck naar rechts is geduwd, gaat het charachter ook naar rechts
		} else if (x < 90 && charachterx > 15) {	//de y coordinaten van de charachter mag niet lager worden dan 15, anders gaat hij uit het scherm
		charachterx--;								//als de nunchuck naar links is geduwd, gaat het charachter ook naar links
	}
	
	
	
	//teken het charachter
	screen.fillCircle(charachterx, y, 10, Color);						//hoofd
	screen.fillRoundRect(charachterx - 16, y - 5, 32, 10, 5, Color);	//schouders
	
	
	//een lijn om het charachter heen tekenen in dezelfde kleur als de achtergrond
	// B = boven, O = onder, L = links, R = rechts, hoofd is de middelste circel en de schouders zijn de uitsteksels aan de linker en rechter kant
	screen.drawLine(charachterx - 4, y - 11, charachterx + 4, y - 11, BACKGROUND);									// B hoofd
	screen.fillTriangle(charachterx - 4, y - 10, charachterx - 8, y - 6, charachterx - 8, y - 10, BACKGROUND);		// LB hoofd
	screen.drawLine(charachterx - 12, y - 6, charachterx - 8, y - 6, BACKGROUND);									// L schouder B
	screen.fillTriangle(charachterx - 13, y - 6, charachterx - 16, y - 2, charachterx - 16, y - 6, BACKGROUND);		// L shouder LB
	screen.drawLine(charachterx - 16, y + 2, charachterx - 16, y - 2, BACKGROUND);									// L schouder L
	screen.fillTriangle(charachterx - 13, y + 6, charachterx - 16, y + 2, charachterx - 16, y + 6, BACKGROUND);		// L schouder LO
	screen.drawLine(charachterx - 12, y + 6, charachterx - 8, y + 6, BACKGROUND);									// L schouder O
	screen.fillTriangle(charachterx - 4, y + 10, charachterx - 8, y + 6, charachterx - 8, y + 10, BACKGROUND);		// LO hoofd
	screen.drawLine(charachterx - 4, y + 11, charachterx + 4, y + 11, BACKGROUND);									// O hoofd
	screen.fillTriangle(charachterx + 8, y + 6, charachterx + 4, y + 11, charachterx + 8, y + 11, BACKGROUND);		// RO hoofd
	screen.drawLine(charachterx + 12, y + 6, charachterx + 8, y + 6, BACKGROUND);									// R schouder O
	screen.fillTriangle(charachterx + 13, y + 6, charachterx + 16, y + 2, charachterx + 16, y + 6, BACKGROUND);		// R schouder RO
	screen.drawLine(charachterx + 16, y + 2, charachterx + 16, y - 2, BACKGROUND);									// R schouder R
	screen.fillTriangle(charachterx + 13, y - 6, charachterx + 16, y - 2, charachterx + 16, y - 6, BACKGROUND);		// R shouder RB
	screen.drawLine(charachterx + 12, y - 6, charachterx + 8, y - 6, BACKGROUND);									// R schouder B
	screen.fillTriangle(charachterx + 4, y - 10, charachterx + 8, y - 6, charachterx + 8, y - 10, BACKGROUND);		// RB hoofd

	//tweede keer de lijn tekenen maar dan 1px meer naar buiten, zodat echt alles van het charachter word weggehaald wat buiten de lijn komt
	screen.drawLine(charachterx - 5, y - 12, charachterx + 5, y - 12, BACKGROUND);		// B hoofd
	screen.drawLine(charachterx - 13, y - 7, charachterx - 9, y - 7, BACKGROUND);		// L schouder B
	screen.drawLine(charachterx - 17, y + 3, charachterx - 17, y - 3, BACKGROUND);		// L schouder L
	screen.drawLine(charachterx - 13, y + 7, charachterx - 9, y + 7, BACKGROUND);		// L schouder O
	screen.drawLine(charachterx - 5, y + 12, charachterx + 4, y + 12, BACKGROUND);		// O hoofd
	screen.drawLine(charachterx + 13, y + 7, charachterx + 9, y + 7, BACKGROUND);		// R schouder O
	screen.drawLine(charachterx + 17, y + 4, charachterx + 17, y - 3, BACKGROUND);		// R schouder R
	screen.drawLine(charachterx + 13, y - 7, charachterx + 9, y - 7, BACKGROUND);		// R schouder B
	
}




//______________________________________________________________\\
//																\\
//						-	NUNCHUCK	-						\\
//______________________________________________________________\\

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
