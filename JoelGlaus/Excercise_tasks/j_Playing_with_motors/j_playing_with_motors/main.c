/* Includes für den Zugriff auf die NIBO Burger Bibliothek */

#include "niboburger/robomain.h"


int speed;

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	
	led_init();						// Anweisung um Led zu initialisieren
	analog_init();					// Anweisung um Analog Signale zu initialisieren
	motpwm_init();					// Anweisung um Motoren zu initialisieren
	speed = 0;
}


void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */

	char key = key_get_char();

	switch (key)
	{
		case 'a':
		speed = speed + 200;
		break;
		
		case 'b':
		speed = speed == 0;
		break;

		case 'c':
		speed = speed - 200;
		break;
	}

	motpwm_setLeft(speed);
	
	motpwm_setRight(speed);

	delay(10);
}