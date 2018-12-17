// 
// 
// 

#include "charachter.h"

charachter::characher(){
	
}

charachter::drawcharacter(int x, int y, int Color){
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