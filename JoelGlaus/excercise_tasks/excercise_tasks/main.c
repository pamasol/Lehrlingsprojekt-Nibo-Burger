/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	led_init();							// Anweisung um Led zu initialisieren
}

void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
	int LedNr;
	
	for (LedNr=1; LedNr<=4; LedNr+=1)	// führt Anweisungen mehrmals durch 
	{
	led_set(LedNr,1);					// Led auf Zustand 1 setzen
	delay(500);							// Zeitverzögerung um 500ms
	led_set(LedNr,0);					// Led auf Zustand 0 setzen
	delay(500);							// Zeitverzögerung um 500ms
	}
}