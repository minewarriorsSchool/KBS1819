// 
// 
// 

#include "Homescreen.h"

Homescreen::Homescreen(){
		nunchuck_get_data();	//nunchuck data ophalen
		joyy = nunchuck_joyy();	//y data van de joystick ophalen
		joyx = nunchuck_joyx();	//x data van de joystick ophalen
		
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

Homescreen::clearScreen(){
	if (scherm != oudescherm) {			//het scherm wor eerst geleegd als je van scherm veranderd
		screen.fillScreen(BACKGROUND);	
		oudescherm = scherm;
	}
}

Homescreen::drawHomescreen(){
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

Homescreen::drawcharHomescreen(int x, int y, int color){
	screen.fillCircle(x, y, 10, color);
	screen.fillRoundRect(x - 16, y - 5, 32, 10, 5, color);
}

Homescreen::drawPointer(){
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

Homescreen::start(){
	drawcharacter(joyx, 275, BLUE);
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}	
}

Homescreen::highscore(){
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("HIGHSCORES");
	
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}	
}

Homescreen::controls(){
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