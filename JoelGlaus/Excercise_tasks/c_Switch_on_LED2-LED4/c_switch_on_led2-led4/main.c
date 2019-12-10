
/* Includes f�r den Zugriff auf die NIBO Burger Bibliothek */

#include "niboburger/robomain.h"


void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */

	led_init();									//Anweisung um Led zu initialisieren
}


void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausf�hren soll */
	
	int ledNr;
	for (ledNr=2; ledNr<=4; ledNr+=1)			// f�hrt Anweisungen mehrmals durch
	
	{
		led_set(ledNr, 1);						// Led auf Zustand 1 setzen
		delay(1000);							// Zeitverz�gerung um 1000ms um Vorgang besser zu beobachten
	}
}