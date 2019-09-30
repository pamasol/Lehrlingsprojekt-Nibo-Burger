<<<<<<< HEAD
/* Includes f�r den Zugriff auf die NIBO Burger Bibliothek */
=======
/* Includes fÃ¼r den Zugriff auf die NIBO Burger Bibliothek */
>>>>>>> 2af2e304d6b73d4bcf5158ebf762741442118bc6
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
<<<<<<< HEAD
	led_init();							// Anweisung um Led zu initialisieren
=======
	led_init();			// Anweisung um Led zu initialisieren
>>>>>>> 2af2e304d6b73d4bcf5158ebf762741442118bc6
}

void loop()
{
<<<<<<< HEAD
	/* Programmcode, den der NIBO Burger immer wieder ausf�hren soll */
	int LedNr;
	
	for (LedNr=1; LedNr<=4; LedNr+=1)	// f�hrt Anweisungen mehrmals durch 
	{
	led_set(LedNr,1);					// Led auf Zustand 1 setzen
	delay(500);							// Zeitverz�gerung um 500ms
	led_set(LedNr,0);					// Led auf Zustand 0 setzen
	delay(500);							// Zeitverz�gerung um 500ms
	}
}
=======
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
	led_set(1,1);			// Led 1 (rote Led) auf Zustand 1 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(1,0);			// Led 1 (rote Led) auf Zustand 0 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(2,1);			// Led 2 (blaue Led) auf Zustand 1 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(2,0);			// Led 2 (blaue Led) auf Zustand 0 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(3,1);			// Led 3 (blaue Led) auf Zustand 1 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(3,0);			// Led 3 (blaue Led) auf Zustand 0 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(4,1);			// Led 4 (rote Led) auf Zustand 1 setzen
	delay(500);				// Zeitverzögerung um 500ms
	led_set(4,0);			// Led 4 (rote Led) auf Zustand 0 setzen
	delay(500);				// Zeitverzögerung um 500ms
}
>>>>>>> 2af2e304d6b73d4bcf5158ebf762741442118bc6
