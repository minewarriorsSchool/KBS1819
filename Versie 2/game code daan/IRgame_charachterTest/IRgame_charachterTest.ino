#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <LiquidCrystal.h>



#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD6BA

#define TFT_DC 9
#define TFT_CS 10

int joyx;		//de x-coordinaten van het charcter
int joyy = 275;		//de y-coordinaten van het charcter


int charachterx = 115; //wordt gebruikt om een vetraging te creeëren waardoor de speler niet binnen 1 seconde naar de andere kant van het scherm kan
int oudcharachterx;	//de oude x-coordinaten

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);

int main (){
	init();
	screen.begin();
	screen.fillScreen(BLACK);
	nunchuck_setpowerpins();
	nunchuck_init();
	Serial.begin(9600);
	
	
	
	drawcharacter(joyx, joyy, BLUE);

	//screen.drawPixel(charachterx, joyy - 10, WHITE);
	
screen.drawLine(charachterx - 4, joyy - 11, charachterx + 4, joyy - 11, WHITE);									// B hoofd
screen.fillTriangle(charachterx - 4, joyy - 10, charachterx - 8, joyy - 6, charachterx - 8, joyy - 10, WHITE);	// LB hoofd
screen.drawLine(charachterx - 12, joyy - 6, charachterx - 8, joyy - 6, WHITE);									// L schouder B
screen.fillTriangle(charachterx - 13, joyy - 6, charachterx - 16, joyy - 2, charachterx - 16, joyy - 6, WHITE);	// L shouder LB
screen.drawLine(charachterx - 16, joyy + 2, charachterx - 16, joyy - 2, WHITE);									// L schouder L
screen.fillTriangle(charachterx - 13, joyy + 6, charachterx - 16, joyy + 2, charachterx - 16, joyy + 6, WHITE);	// L schouder LO
screen.drawLine(charachterx - 12, joyy + 6, charachterx - 8, joyy + 6, WHITE);									// L schouder O
screen.fillTriangle(charachterx - 4, joyy + 10, charachterx - 8, joyy + 6, charachterx - 8, joyy + 10, WHITE);	// LO hoofd
screen.drawLine(charachterx - 4, joyy + 11, charachterx + 4, joyy + 11, WHITE);									// O hoofd
screen.fillTriangle(charachterx + 8, joyy + 6, charachterx + 4, joyy + 11, charachterx + 8, joyy + 11, WHITE);	// RO hoofd
screen.drawLine(charachterx + 12, joyy + 6, charachterx + 8, joyy + 6, WHITE);									// R schouder O
screen.fillTriangle(charachterx + 13, joyy + 6, charachterx + 16, joyy + 2, charachterx + 16, joyy + 6, WHITE);	// R schouder RO
screen.drawLine(charachterx + 16, joyy + 2, charachterx + 16, joyy - 2, WHITE);									// R schouder R
screen.fillTriangle(charachterx + 13, joyy - 6, charachterx + 16, joyy - 2, charachterx + 16, joyy - 6, WHITE);	// R shouder RB
screen.drawLine(charachterx + 12, joyy - 6, charachterx + 8, joyy - 6, WHITE);									// R schouder B
screen.fillTriangle(charachterx + 4, joyy - 10, charachterx + 8, joyy - 6, charachterx + 8, joyy - 10, WHITE);	// RB hoofd


screen.drawLine(charachterx - 5, joyy - 12, charachterx + 5, joyy - 12, WHITE);		// B hoofd
screen.drawLine(charachterx - 13, joyy - 7, charachterx - 9, joyy - 7, WHITE);		// L schouder B
screen.drawLine(charachterx - 17, joyy + 3, charachterx - 17, joyy - 3, WHITE);		// L schouder L
screen.drawLine(charachterx - 13, joyy + 7, charachterx - 9, joyy + 7, WHITE);		// L schouder O
screen.drawLine(charachterx - 5, joyy + 12, charachterx + 4, joyy + 12, WHITE);		// O hoofd
screen.drawLine(charachterx + 13, joyy + 7, charachterx + 9, joyy + 7, WHITE);		// R schouder O
screen.drawLine(charachterx + 17, joyy + 4, charachterx + 17, joyy - 3, WHITE);		// R schouder R
screen.drawLine(charachterx + 13, joyy - 7, charachterx + 9, joyy - 7, WHITE);		// R schouder B




	for(;;) {
		
		nunchuck_get_data();	//input van de nunchuck ophalen
		joyx = nunchuck_joyx() - 16; // de input vertalen naar nuttige x coordinaten
		
		
		
		
		
		
		
	}
}



void drawcharacter(int x, int y, int Color){
	
	
	if (x > 140 && charachterx < 225) {
		charachterx++;								//als de nunchuck naar rechts is geduwd, gaat het charachter ook naar rechts
		} else if (x < 90 && charachterx > 15) {
		charachterx--;								//als de nunchuck naar links is geduwd, gaat het charachter ook naar links
	}
	
	
	
	//teken het charachter
	screen.fillCircle(charachterx, y, 10, Color);
	screen.fillRoundRect(charachterx - 16, joyy - 5, 32, 10, 5, Color);
	
	//verwijder de vorige locatie van het charachter
	if(oudcharachterx != charachterx){ //zodat het verwijderen niet gebeurt terwijl het charachter stil staat
		if(oudcharachterx <= charachterx){									//teken een zwart charachter op de vorige locatie van het charachter
			screen.fillCircle(oudcharachterx -1, y, 10, BLACK);
			screen.fillRoundRect(oudcharachterx - 17, joyy - 5, 32, 13, 5, BLACK);
		}
		
		if(oudcharachterx >= charachterx){									//teken een zwart charachter op de vorige locatie van het charachter
			screen.fillCircle(oudcharachterx + 1, y, 10, BLACK);
			screen.fillRoundRect(oudcharachterx - 15, joyy - 5, 32, 13, 5, BLACK);
		}
	}
	oudcharachterx = charachterx;	//oude locatie opslaan
}







//________________________________________________________________________________________________

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

int nunchuck_y()

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
