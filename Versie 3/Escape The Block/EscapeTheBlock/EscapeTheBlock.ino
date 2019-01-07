#include <Wire.h>
#include <Adafruit_ILI9341.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD6BA
int colors[8] {BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE, GREY};

#define MAXARRAY 25
#define BACKGROUND	0x0000

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 screen = Adafruit_ILI9341(TFT_CS, TFT_DC);

int rectcolor1 = GREY;
int rectcolor2 = CYAN;
int rectcolor;

int speler1kleur = BLUE;
int speler2kleur = RED;
int MAXLEVENS = 10;			//de hoeveelheid levens die je hebt (er passen er 12 op het scherm)

int joyy, joyx;						// x en y coordinaten van de joystick
int pointerPos = 1;					//de positie van de pointer in het hoofdmenu
int oudpointerPos = 2;				//de oude positie van de pointer in het hoofdmenu
int scherm = 1, oudescherm = 1;		//het scherm wat nu word getoond (hoofdmenu, controls, het spel) en het vorige scherm om veranderingen te controleren

int tijd;				//de tijd die de timer ieder ms met 1 verhoogd
int stap1 = -250;
int stap2;		// de y coordinaten van blok 1 en 2
int blok2;				// bij 1 mag blok2 vallen, bij 0 nog niet
int snelheid = 5;		//snelheid waarmee de blokken vallen (2 = heel snel, 4 = langzaam)
int blokweg = 10;		// de grootte van het zwarte vierkant wat de blokken achtervolgt om het spoor weg te halen


int checkcollision1;	// bij 0 is er geen collision, bij 1 wel
int checkcollision2;
int collisiontijd1;		// de score die de speler had toen de collision plaatsvond
int collisiontijd2;
int invinccheck1;		// check die ervoor zorgt dat de invincibility maar 1 keer word geregeld, ipv idere loep weer
int invinccheck2;


int geraakt1;
int geraakt2;
int score;
int extrascore;
int livespointer;		// bij 0 is de linker pijl in het settings menu geselecteerd, bij 1 de rechter pijl en bij 2 geen


int locaties [MAXARRAY], groottes [MAXARRAY];
int randomlocatie, randomgrootte, randomafstand = 180;
int nummer1 = 1, nummer2 = MAXARRAY / 2;
int timer1_counter;
int nunchuckcheck;
int zcheck = 1;

int player1x;			//de x-coordinaten van speler 1 en 2
int player2x;
int player1y = 275;		//de y-coordinaten van speler 1 en 2
int player2y = 300;
int playerybuffer;		// word gebruikt voor het omwisselen van player1y en player2y

int charachterx2 = 115;
int charachterx = 115;	//de x locatie van het charchter, als de nunchuck naar rechts/links word geduwd komt er iedere loop 1 bij/af
//115 is de beginlocatie waaruit de charachter wordt getekend
int spelersnelheid = 3;		// snelheid waarmee de spelers van links naar rechts gaan

//highscore:
boolean nieuwHighscore = false;
int Counter = 0;
int opslagHigscore1;
int opslagHigscore2;


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
	
	Serial.begin(9600);
	screen.begin();
	screen.fillScreen(BACKGROUND);
	
	nunchuck_setpowerpins();
	nunchuck_init();			//nunchuck initialiseren
	
	srand(3);
	seed();
	
	screen.fillScreen(BACKGROUND);
	
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
			nunchuckcheck = 0;
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
		if (nunchuckcheck == 0) {		// het scherm weer eenmaal zwart maken om de tekst "please connect nunchuck" weg te halen
			nunchuckcheck = 1;
			screen.fillScreen(BACKGROUND);
		}
		
		
		clearScreen();			// als het scherm is veranderd alles eerst zwart maken
		
		
		if(scherm == 1){		//scherm 1 is het homescreen
			drawHomescreen();
			drawPointer();
		}
		
		clearScreen();
		
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
		
		if (scherm == 6) {
			settings();
		}
		
	}
}

void clearScreen (){
	if (scherm != oudescherm) {			//het scherm word geleegd als je van scherm veranderd
		screen.fillScreen(BACKGROUND);
		if(scherm == 1) {				//het spel is terug gegaan naar het hoofdmenu dus alles word gereset naar de beginwaardes
			stap1 = -200;
			stap2 = 0;
			charachterx = 115;
			charachterx2 = 115;
			geraakt2 = 0;
			geraakt1 = 0;
			score = 0;
			extrascore = 0;
			blok2 = 0;
			nummer1 = 1;
			nummer2 = MAXARRAY / 2;
			snelheid = 5;
			blokweg = 10;
			checkcollision1 = 0;
			checkcollision2 = 0;
			collisiontijd1 = 0;
			collisiontijd2 = 0;
			Counter = 0;
			nieuwHighscore = false;
			spelersnelheid = 3;
			rectcolor1 = GREY;
			rectcolor2 = CYAN;
			seed();					// nieuwe seed berekenen
			if (oudescherm == 5 || oudescherm == 2) {		//als het vorige scherm het gameover scherm of het spel was en we zitten nu in het menu
				playerybuffer = player1y;		//locatie van de spelers omwisselen zodat niet steeds 1 speler een voordeel heeft
				player1y = player2y;
				player2y = playerybuffer;
			}
		}
		if (scherm == 4) {
			snelheid = 10;
			spelersnelheid = 5;
		}
		oudescherm = scherm;
	}
}

void drawHomescreen(){				//het homescreen
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);  
	screen.setTextSize(3);
	screen.println("Escape the");
	screen.setCursor(77, 70);
	screen.println("BLOCK");
	screen.setCursor(90, 130);
	screen.setTextSize(2);
	screen.println("Start");
	screen.setCursor(65, 150);
	screen.println("Highscore");
	screen.setCursor(71, 171);
	screen.println("Controls");
	screen.setCursor(70, 191);
	screen.println("Settings");
	
	
	speler1kleur = BLUE;
	speler2kleur = RED;
	
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
		
		if (oudpointerPos == 4){
			screen.fillTriangle(45, 191, 45, 201, 55, 196, BACKGROUND);
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
	
	if (pointerPos == 4){
		screen.fillTriangle(45, 191, 45, 201, 55, 196, GREEN);	//teken een pointer op positie 3
		
		if(nunchuck_zbutton() == 1){
			scherm = 6;		//als de pointer op positie 4 is en er word op z gedrukt, ga naar scherm 2 (settings)
		}
	}
	
	oudpointerPos = pointerPos;
	
	_delay_ms(50);
	

	if (joyy > 160 && pointerPos > 1) {
		pointerPos--;
	} else if (joyy < 80 && pointerPos < 4) {
		pointerPos++;
	}
	
}

void start(){
	/*
	Serial.print("nummer1: ");
	Serial.print(nummer1);
	Serial.print(",   stap1: ");
	Serial.print(stap1);
	Serial.print("  \t nummer2: ");
	Serial.print(nummer2);
	Serial.print(",   stap2: ");
	Serial.println(stap2);
	*/
	
	player1x = nunchuck_joyx();	//x data van de joystick ophalen
	
	
	if (MAXLEVENS - geraakt1 > 0) {	
		drawcharacter(player1x, player1y, speler1kleur);		//teken speler 1
	}
	/*
	if (MAXLEVENS - geraakt2 > 0) {	
		player2x = 125 - player1x + 125;
		drawcharacter2(player2x, player2y, speler2kleur);	//teken speler 2
	} 
	*/

	// als de array met random locaties en groottes voorbij is moet het scherm even leeg zijn en moet er een nieuwe array worden gemaakt
	if (nummer2 != MAXARRAY) {
		if (nummer2 != MAXARRAY - 1) {								//blok1 niet tekenen als de array bijna afgelopen is
			drawblock(locaties[nummer1], groottes[nummer1]);		//teken blok1
		} else if (stap1 < 340 + groottes[nummer1]) {				// zorg ervoor dat blok1 wel helemaal naar beneden valt, als het blok beneden is geen nieuwe meer tekenen totdat er een nieuwe array is
			drawblock(locaties[nummer1], groottes[nummer1]);		
		}
		drawblock2(locaties[nummer2], groottes[nummer2]);				//teken blok2
	} else {
		stap1 = 0;
		stap2 = 0;
		blok2 = 0;
		nummer1 = 1;							//reset
		nummer2 = MAXARRAY / 2;
		extrascore = extrascore + MAXARRAY;
		seed();
	}
	
	collision();		//check of er een botsing is
	header();			//teken de header
	
	if (MAXLEVENS - geraakt1 <= 0 && MAXLEVENS - geraakt2 <= 0) {		//als de levens op zijn, ga naar het gameover scherm
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
	
	if (MAXLEVENS < 4) {
		for(Counter; Counter < 5 && score != EEPROM_read(Counter); Counter++){		//Dankzij de counter gaat de FOR maar 5 keer door. De tweede voorwaarde zorgt ervoor dat als de nieuwe highscore al bestaat, de FOR direct stopt.
			if(score > EEPROM_read(Counter) && !nieuwHighscore){			//Voorwaarde1: nieuwe score moet hoger zijn dan de al opgeslagen score. Voorwaarde2: Er moet nog geen highscore veranderd zijn
				opslagHigscore1 = EEPROM_read(Counter + 1);				//Opslaan van de score die onder de score staat die wordt veranderd
				opslagHigscore2 = EEPROM_read(Counter + 2);				//Opslaan van de daar op volgende score
				EEPROM_write(Counter + 1, EEPROM_read(Counter));		//Nieuwe highscore wordt toegepast
				EEPROM_write(Counter + 4, EEPROM_read(Counter + 3));	//De daaropvolgende wordt gelijk gemaakt aan de oude waarde van score 1
				EEPROM_write(Counter, score);							//De daaropvolgende wordt gelijk gemaakt aan de oude waarde van score 2
				EEPROM_write(Counter + 2, opslagHigscore1);				//De daaropvolgende wordt gelijk gemaakt aan de oude waarde van score 3
				EEPROM_write(Counter + 3, opslagHigscore2);				//De daaropvolgende wordt gelijk gemaakt aan de oude waarde van score 4
				nieuwHighscore = true;									//De mogelijkheid voor een nieuwe highscore wordt weer open gezet
			}
		}
	}
	
		
	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}

void highscore(){
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("HIGHSCORES");
	screen.setCursor(60, 90);
	screen.print("1:  ");
	screen.println(EEPROM_read(0));
	screen.setCursor(60, 120);
	screen.print("2:  ");
	screen.println(EEPROM_read(1));
	screen.setCursor(60, 150);
	screen.print("3:  ");
	screen.println(EEPROM_read(2));
	screen.setCursor(60, 180);
	screen.print("4:  ");
	screen.println(EEPROM_read(3));
	screen.setCursor(60, 210);
	screen.print("5:  ");
	screen.println(EEPROM_read(4));
	
	
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
	screen.setTextSize(2);
	screen.println("Move the joystick");
	screen.setCursor(30, 100);
	screen.println("to go left and");
	screen.setCursor(30, 120);

	drawcharacter(joyx, 160, RED);

	screen.println("right");
	screen.setCursor(30, 190);
	screen.println("Press C to go");
	screen.setCursor(30, 210);
	screen.println("back to the");
	screen.setCursor(30, 230);
	screen.println("homescreen");

	if(nunchuck_cbutton() == 1){
		scherm = 1;
	}
}

void settings () {
	screen.setCursor(30, 40);
	screen.setTextColor(WHITE);
	screen.setTextSize(3);
	screen.println("SETTINGS");
	
	screen.setCursor(30, 80);
	screen.setTextSize(2);
	screen.println("How many lives");
	screen.setCursor(30, 100);
	screen.println("do you want?");
		
		
	if (joyx > 160) {
		livespointer = 1;		//als de nunchuck naar rechts is geduwd, word het rechter pijltje geselecteerd
	} else if (joyx < 70) {
		livespointer = 0;		//bij rechts word het rechter pijltje geselecteerd
	}
		
		
	if (livespointer == 2) {	//geen van de pijlen is nog geselecteerd, ze zijn allebei wit
		
		screen.fillTriangle(90, 131, 90, 141, 80, 136, WHITE);
		screen.fillTriangle(140, 131, 140, 141, 150, 136, WHITE);
		
	} else if (livespointer == 1) {		// de rechter pijl is geselecteerd
		
		screen.fillTriangle(90, 131, 90, 141, 80, 136, WHITE);
		screen.fillTriangle(140, 131, 140, 141, 150, 136, GREEN);		//de rechter pijl is geselecteerd (groen)
		
		if(nunchuck_zbutton() == 1 && MAXLEVENS != 99 && zcheck == 0){		// als er op z word gedrukt worden de maxlevens verhoogd met 1
			MAXLEVENS++;
			screen.fillRect(95, 131, 50, 15, BACKGROUND);	//oude maxlevens weghalen
			zcheck = 1;										// zcheck zorgt ervoor dat je de knop apart moet indrukken voor iedere keer dat je er een leven bij of af wilt
		}
		
	} else if (livespointer == 0) {
		
		screen.fillTriangle(90, 131, 90, 141, 80, 136, GREEN);			//de linker pijl is geselecteerd (groen)
		screen.fillTriangle(140, 131, 140, 141, 150, 136, WHITE);
		
		if(nunchuck_zbutton() == 1 && MAXLEVENS != 1 && zcheck == 0){
			MAXLEVENS--;
			screen.fillRect(95, 131, 50, 15, BACKGROUND);
			zcheck = 1;
		}
	}
	
	
	screen.setCursor(105, 131);
	screen.println(MAXLEVENS);
	
	if (MAXLEVENS > 3) {
		screen.setTextSize(1);
		screen.setCursor(20, 160);
		screen.println("If you start with more than 3 lives,");
		screen.setCursor(20, 170);
		screen.println("your highscore won't be saved");
	} else {
		screen.fillRect(10, 159, 240, 20, BACKGROUND);
	}
		
	
	if(nunchuck_zbutton() == 0){
		zcheck = 0;					//pas als de z knop word losgelaten kan er weer een leven bijkomen
	}
	
	if(nunchuck_cbutton() == 1){
		livespointer = 2;
		scherm = 1;
	}
}


void drawblock(int x, int grootte) {
	if (grootte > 50) {
		grootte = 50;
	}
	
	
	screen.fillRect(x, stap1 - grootte, grootte, grootte, rectcolor1);
	screen.fillRect(x, stap1 - grootte - blokweg, grootte, blokweg, BACKGROUND);
		
	if (stap1 > 330 + grootte && stap2 > randomafstand){
		nummer1++;
		screen.fillRect(200, 0, 50, 10, BACKGROUND);		//oude score weghalen om plaats te maken voor de nieuwe score
		stap1 = 0;
		if (score >= 100) {
			rectcolor1 = colors[rectcolor];
		}
	}
}

void drawblock2(int x, int grootte) {

	if (stap1 >= 170) {
		blok2 = 1;		//als het aller eerste blok1 weg is mag blok2 ook beginnen
	}
	
	if (grootte > 50) {
		grootte = 50;
	}
	
	if (blok2 == 1) {
		screen.fillRect(x, stap2, grootte, grootte, rectcolor2);			//blok2 tekenen
		screen.fillRect(x, stap2 - blokweg, grootte, blokweg, BACKGROUND);
		
		if (stap2 > (330 + grootte) && stap1 > randomafstand){	// als het vorige blok2 uit het scherm is en blok1 is ver genoeg
			nummer2++;											// volgende blok2
			screen.fillRect(200, 0, 50, 10, BACKGROUND);		// oude score weghalen om plaats te maken voor de nieuwe score
			stap2 = 0;											//stappenteller reseten
			randomafstand = rand() % 20 + 180;					// nieuwe random afstand uitrekenen die tussen de blokken moet zitten
			
			if (score >= 100) {							// na 100 blokken veranderd de kleur van de blokken
				rectcolor2 = colors[rectcolor + 1];
				rectcolor = rectcolor + 2;
				if (rectcolor > 5) {					// na 8 kleuren weer opnieuw beginnen met de array
					rectcolor = 0;
				}
			}
			
		}
	}  else {
		screen.fillRect(x, stap2 - grootte, grootte, grootte, RED);
		screen.fillRect(x, stap2 - blokweg, grootte, blokweg, BACKGROUND);
	}
	
	
	if (score >= 175) {				//de snelheid van de blokken, de grootte van het weghaal blok en de spelersnelheid worden verhoogd bij ierder 25 of 50 punten
		snelheid = 0;				
		blokweg = 50;
		spelersnelheid = 7;
	} else if (score >= 125) {
		snelheid = 1;
		blokweg = 35;				// de grootte van het zwarte blok wat de blokken achtervolgt om het spoor weg te halen
		spelersnelheid = 5;			// snelheid van de speler ( 7 = snel, 3 = sloom)
	} else if (score >= 75) {
		snelheid = 2;				// de blokken vallen heel snel
		blokweg = 25;
		spelersnelheid = 4;
	} else if (score >= 50) {
		snelheid = 3;				// de blokken vallen minder snel
		blokweg = 22;
		spelersnelheid = 4;
	} else if (score >= 25) {
		snelheid = 4;				// de blokken vallen sloom
		blokweg = 19;				
		spelersnelheid = 3;
	}
}

void seed (){
	for(int i = 0; i <= MAXARRAY; i++) {		//vul 2 arrays met random groottes en locaties voor de blokken
		groottes[i] = rand() % 25 + 25;
		/*		UITLEZEN
		Serial.print(i);
		Serial.print(": ");
		Serial.print(groottes[i]);
		Serial.print(",  ");
		*/
		locaties[i] = rand() % (240 - groottes[i]);
	} 
	//Serial.println("");
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
	
	if (MAXLEVENS - geraakt1 > 0){
		screen.print("lives");
	} else {
		screen.print("dead");
	}
	
	for(int lev = 40; (lev < (MAXLEVENS - geraakt1) * 10 + 40) && lev < 160; lev = lev + 10){	//het teken van alle levens
		screen.fillCircle(lev, 4, 3, RED);
	}
	
	screen.setCursor(0, 10);
	
	if (MAXLEVENS - geraakt2 > 0){
		screen.print("lives");
	} else {
		screen.print("dead");
	}
	
	for(int lev = 40; (lev < (MAXLEVENS - geraakt2) * 10 + 40) && lev < 160; lev = lev + 10){	//het teken van alle levens
		screen.fillCircle(lev, 14, 3, RED);
	}
	
	screen.setCursor(160, 0);
	screen.print("score: ");
	score = nummer1 + (nummer2 - MAXARRAY / 2) + extrascore;
	screen.print(score);
	
}

void collision (){
	if (checkcollision1 == 0){
		//charachter 1
		if(locaties[nummer1] + groottes[nummer1] >= charachterx - 15 && locaties[nummer1] <= charachterx - 15 && (stap1 >= player1y - 10 && stap1 <= player1y + 10)) {	// controleren of blok1 de linker kant van de speler heeft geraakt
			checkcollision1 = 1;		//niet meer checken of er een botsing is
			collisiontijd1 = score;	//de tijd van de collsion opslaan zodat de speler voor een korte tijd invincible is
			geraakt1++;				// de speler is één keer vaker geraakt en verliest dus ook één leven
		}
		
		if(locaties[nummer1] + groottes[nummer1] >= charachterx + 15 && locaties[nummer1] <= charachterx + 15 && (stap1 >= player1y - 10 && stap1 <= player1y + 10)) {	// controleren of blok1 de rechter kant van de speler heeft geraakt
			checkcollision1 = 1;
			collisiontijd1 = score;
			geraakt1++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx - 15 && locaties[nummer2] <= charachterx - 15 && (stap2 >= player1y - 10 && stap2 <= player1y + 10)) {	// controleren of blok2 de linker kant van de speler heeft geraakt
			checkcollision1 = 1;
			collisiontijd1 = score;
			geraakt1++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx + 15 && locaties[nummer2] <= charachterx + 15 && (stap2 >= player1y - 10 && stap2 <= player1y + 10)) {	// controleren of blok2 de rechter kant van de speler heeft geraakt
			checkcollision1 = 1;
			collisiontijd1 = score;
			geraakt1++;
		}
		invinccheck1 = 0;
	}
	
	// 275 - 300
	if (checkcollision2 == 0){		// collision voor de tweede speler
		// charachter 2
		if(locaties[nummer1] + groottes[nummer1] >= charachterx2 - 15 && locaties[nummer1] <= charachterx2 - 15 && (stap1 >= player2y - 10 && stap1 <= player2y + 10)) {
			checkcollision2 = 1;
			collisiontijd2 = score;
			geraakt2++;
		}
		
		if(locaties[nummer1] + groottes[nummer1] >= charachterx2 + 15 && locaties[nummer1] <= charachterx2 + 15 && (stap1 >= player2y - 10 && stap1 <= player2y + 10)) {
			checkcollision2 = 1;
			collisiontijd2 = score;
			geraakt2++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx2 - 15 && locaties[nummer2] <= charachterx2 - 15 && (stap2 >= player2y - 10 && stap2 <= player2y + 10)) {
			checkcollision2 = 1;
			collisiontijd2 = score;
			geraakt2++;
		}
		
		if(locaties[nummer2] + groottes[nummer2] >= charachterx2 + 15 && locaties[nummer2] <= charachterx2 + 15 && (stap2 >= player2y - 10 && stap2 <= player2y + 10)) {
			checkcollision2 = 1;
			collisiontijd2 = score;
			geraakt2++;
		}
		invinccheck2 = 0;
	}
	
	//speler 1
	if (checkcollision1 == 1 && invinccheck1 == 0) {	//er is een botsing geweest door speler 1
		blokweg = blokweg + 3;								//het weghalen van de blokken word een klein beetje opgehoogd
		screen.fillRect(0, 0, 160, 10, BACKGROUND);			// de oude levens weg halen zodat de nieuwe levens eroverheen kunnen wprden getekend met 1 hartje minder
		speler1kleur = WHITE;								// de kleur van de speler word wit zolang dezen invincible is
		invinccheck1 = 1;									// invinccheck word weer op 1 gezet zodat deze if maar 1 keer true is
	}
	
	if (checkcollision1 == 1 && collisiontijd1 + 1 == score) {
		checkcollision1 = 0;								// invincibility weer voorbij, controleren op collision weer van start
		speler1kleur = BLUE;
	}
	
	
	//speler 2
	if (checkcollision2 == 1 && invinccheck2 == 0) {	//er is een botsing geweest door speler 2
		blokweg = blokweg + 3;								
		screen.fillRect(0, 0, 160, 25, BACKGROUND);
		speler2kleur = WHITE;
		invinccheck2 = 1;
	}
	
	if (checkcollision2 == 1 && collisiontijd2 + 1 == score) {
		checkcollision2 = 0;
		speler2kleur = RED;
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
	//speler 1 tekenen
	
	if (x > 140 && charachterx < 225) {			//de x coordinaten van de charachter mag niet hoger worden dan 225, anders gaat hij uit het scherm
		charachterx = charachterx + spelersnelheid;								//als de nunchuck naar rechts is geduwd, gaat het charachter ook naar rechts
	} else if (x < 118 && charachterx > 15) {	//de y coordinaten van de charachter mag niet lager worden dan 15, anders gaat hij uit het scherm
		charachterx = charachterx - spelersnelheid;								//als de nunchuck naar links is geduwd, gaat het charachter ook naar links
	}
	
	//teken het charachter
	screen.fillCircle(charachterx, y, 10, Color);						//hoofd
	screen.fillRoundRect(charachterx - 16, y - 5, 32, 10, 5, Color);	//schouders
	
	
	//een lijn om het charachter heen tekenen in dezelfde kleur als de achtergrond
	// B = boven, O = onder, L = links, R = rechts, hoofd is de middelste circel en de schouders zijn de uitsteksels aan de linker en rechter kant
	if (x > 140) {					//als de player naar rechts gaat, haal links weg
		screen.fillTriangle(charachterx - 4, y - 11, charachterx - 9, y - 6, charachterx - 9, y - 11, BACKGROUND);		// LB hoofd
		screen.fillRect(charachterx - 15, y - 11, 7, 6, BACKGROUND);													// L schouder B
		screen.fillTriangle(charachterx - 13, y - 6, charachterx - 16, y - 2, charachterx - 16, y - 6, BACKGROUND);		// L shouder LB
		screen.fillRect(charachterx - 25, y - 8, 10, 17, BACKGROUND);													// L schouder L
		screen.fillTriangle(charachterx - 13, y + 6, charachterx - 16, y + 2, charachterx - 16, y + 6, BACKGROUND);		// L schouder LO
		screen.fillRect(charachterx - 15, y + 6, 7, 6, BACKGROUND);														// L schouder O
		screen.fillTriangle(charachterx - 4, y + 11, charachterx - 9, y + 6, charachterx - 9, y + 11, BACKGROUND);		// LO hoofd
	} else if (x < 118) {			//als de player naar link gaat, haal rechts weg
		screen.fillTriangle(charachterx + 9, y + 6, charachterx + 4, y + 11, charachterx + 9, y + 11, BACKGROUND);		// RO hoofd
		screen.fillRect(charachterx + 8, y + 6, 7, 6, BACKGROUND);														// R schouder O
		screen.fillTriangle(charachterx + 13, y + 6, charachterx + 16, y + 2, charachterx + 16, y + 6, BACKGROUND);		// R schouder RO
		screen.fillRect(charachterx + 15, y - 8, 10, 17, BACKGROUND);													// R schouder R
		screen.fillTriangle(charachterx + 13, y - 6, charachterx + 16, y - 2, charachterx + 16, y - 6, BACKGROUND);		// R shouder RB
		screen.fillRect(charachterx + 9, y - 11, 7, 6, BACKGROUND);														// R schouder B
		screen.fillTriangle(charachterx + 4, y - 11, charachterx + 9, y - 6, charachterx + 9, y - 11, BACKGROUND);		// RB hoofd
	}
}



void drawcharacter2(int x, int y, int Color){
	// speler 2 tekenen
	
	if (x > 140 && charachterx2 < 225) {			//de x coordinaten van de charachter mag niet hoger worden dan 225, anders gaat hij uit het scherm
		charachterx2 = charachterx2 + spelersnelheid;								//als de nunchuck naar rechts is geduwd, gaat het charachter ook naar rechts
	} else if (x < 118 && charachterx2 > 15) {	//de y coordinaten van de charachter mag niet lager worden dan 15, anders gaat hij uit het scherm
		charachterx2 = charachterx2 - spelersnelheid;								//als de nunchuck naar links is geduwd, gaat het charachter ook naar links
	}
	
	//teken het charachter
	screen.fillCircle(charachterx2, y, 10, Color);						//hoofd
	screen.fillRoundRect(charachterx2 - 16, y - 5, 32, 10, 5, Color);	//schouders
	
	
	//een lijn om het charachter heen tekenen in dezelfde kleur als de achtergrond
	// B = boven, O = onder, L = links, R = rechts, hoofd is de middelste circel en de schouders zijn de uitsteksels aan de linker en rechter kant
	if (x > 140) {				//als de player naar rechts gaat, haal links weg
		screen.fillTriangle(charachterx2 - 4, y - 11, charachterx2 - 9, y - 6, charachterx2 - 9, y - 11, BACKGROUND);		// LB hoofd
		screen.fillRect(charachterx2 - 15, y - 11, 7, 6, BACKGROUND);														// L schouder B
		screen.fillTriangle(charachterx2 - 13, y - 6, charachterx2 - 16, y - 2, charachterx2 - 16, y - 6, BACKGROUND);		// L shouder LB
		screen.fillRect(charachterx2 - 25, y - 8, 10, 17, BACKGROUND);														// L schouder L
		screen.fillTriangle(charachterx2 - 13, y + 6, charachterx2 - 16, y + 2, charachterx2 - 16, y + 6, BACKGROUND);		// L schouder LO
		screen.fillRect(charachterx2 - 15, y + 6, 7, 6, BACKGROUND);														// L schouder O
	} else if (x < 118) {		//als de player naar link gaat, haal rechts weg
		screen.fillTriangle(charachterx2 - 4, y + 11, charachterx2 - 9, y + 6, charachterx2 - 9, y + 11, BACKGROUND);		// LO hoofd
		screen.fillTriangle(charachterx2 + 9, y + 6, charachterx2 + 4, y + 11, charachterx2 + 9, y + 11, BACKGROUND);		// RO hoofd
		screen.fillRect(charachterx2 + 8, y + 6, 7, 6, BACKGROUND);															// R schouder O
		screen.fillTriangle(charachterx2 + 13, y + 6, charachterx2 + 16, y + 2, charachterx2 + 16, y + 6, BACKGROUND);		// R schouder RO
		screen.fillRect(charachterx2 + 15, y - 8, 10, 17, BACKGROUND);														// R schouder R
		screen.fillTriangle(charachterx2 + 13, y - 6, charachterx2 + 16, y - 2, charachterx2 + 16, y - 6, BACKGROUND);		// R shouder RB
		screen.fillRect(charachterx2 + 9, y - 11, 7, 6, BACKGROUND);														// R schouder B
		screen.fillTriangle(charachterx2 + 4, y - 11, charachterx2 + 9, y - 6, charachterx2 + 9, y - 11, BACKGROUND);		// RB hoofd
	}
}








void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}


unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}










//________________________________________________________________//



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