/* 
	Editor: https://www.visualmicro.com/
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 10807
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 10807
#define ARDUINO_AVR_UNO
#define ARDUINO_ARCH_AVR
int main ();
void clearScreen ();
void drawHomescreen();
void drawcharHomescreen(int x, int y, int color);
void drawPointer();
void start();
void gameover();
void highscore();
void controls();
void settings ();
void drawblock(int x, int grootte);
void drawblock2(int x, int grootte);
void seed ();
void header();
void collision ();
void drawcharacter(int x, int y, int Color);
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
static void nunchuck_setpowerpins();
void nunchuck_init();
void nunchuck_send_request();
int nunchuck_get_data();
void nunchuck_print_data();
char nunchuk_decode_byte (char x);
int nunchuck_zbutton();
int nunchuck_cbutton();
int nunchuck_joyx();
int nunchuck_joyy();
int nunchuck_accelx();
int nunchuck_accely();
int nunchuck_accelz();

#include "pins_arduino.h" 
#include "arduino.h"
#include "IRgame_blokkenTest.ino"
