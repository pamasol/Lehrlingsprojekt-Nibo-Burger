/* Includes f�r den Zugriff auf die NIBO Burger Bibliothek */

#include "niboburger/robomain.h"


void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */

	led_init();				// Anweisung um Led zu initialisieren
	led_init();						// Anweisung um Led zu initialisieren
}


void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausf�hren soll */

	led_set(1,1);			// Led 1 (rote Led) auf Zustand 1 setzen
	delay(500);				// Zeitverz�gerung um 500ms
	led_set(1,0);			// Led 1 (rote Led) auf Zustand 0 setzen

	delay(500);				// Zeitverz�gerung um 500ms
	
	led_set(2,1);			// Led 2 (blaue Led) auf Zustand 1 setzen
	delay(500);				// Zeitverz�gerung um 500ms
	led_set(2,0);			// Led 2 (blaue Led) auf Zustand 0 setzen
	
	delay(500);				// Zeitverz�gerung um 500ms
	
	led_set(3,1);			// Led 3 (blaue Led) auf Zustand 1 setzen
	delay(500);				// Zeitverz�gerung um 500ms
	led_set(3,0);			// Led 3 (blaue Led) auf Zustand 0 setzen
	
	delay(500);				// Zeitverz�gerung um 500ms
	
	led_set(4,1);			// Led 4 (rote Led) auf Zustand 1 setzen
	delay(500);				// Zeitverz�gerung um 500ms
	led_set(4,0);			// Led 4 (rote Led) auf Zustand 0 setzen
	
	delay(500);				// Zeitverz�gerung um 500ms
}

/* Programmcode, den der NIBO Burger immer wieder ausführen soll */

int ledNr;

for (ledNr=2; ledNr<=4; ledNr+=1)			// führt Anweisungen mehrmals durch

{

	led_set(ledNr, 1);				// Led auf Zustand 1 setzen
	delay(1000);					// Zeitverzögerung um 1000ms um Vorgang besser zu beobachten
}

}