/* Includes für den Zugriff auf die NIBO Burger Bibliothek */

#include "niboburger/robomain.h"


void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	
	led_init();			// Anweisung um Led zu initialisieren
	analog_init();		// Anweisung um Analog Signale zu initialisieren
}


void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */

	char key = key_get_char();

	switch (key)
	{
		case 'a' :			// Taster 1 nicht betätigt
		led_set(1,0);		// Led 1 (rote Led) auf Zustand 0 setzen
		break;				// Beendung der Switch Anweisung

		case 'A' :			// Taster 1 betätigt
		led_set(1,1);		// Led 1 (rote Led) auf Zustand 1 setzen
		break;				// Beendung der Switch Anweisung

		case 'b' :			// Taster 2 nicht betätigt
		led_set(2,0);		// Led 2 (blaue Led) auf Zustand 0 setzen
		break;				// Beendung der Switch Anweisung

		case 'B' :			// Taster 2 betätigt
		led_set(2,1);		// Led 2 (blaue Led) auf Zustand 1 setzen
		break;				// Beendung der Switch Anweisung

		case 'c' :			// Taster 3 nicht betätigt
		led_set(3,0);		// Led 3 (blaue Led) auf Zustand 0 setzen
		break;				// Beendung der Switch Anweisung

		case 'C' :			// Taster 3 betätigt
		led_set(3,1);		// Led 3 (blaue Led) auf Zustand 1 setzen
		break;				// Beendung der Switch Anweisung

	}
}