/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	led_init();								// Anweisung um Led zu initialisieren
}

void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
	int LedNr;
	int time;
	
	for (time=50; time<=200; time*=2)		// führt Anweisungen mehrmals durch 
	{
		
		for (LedNr=1; LedNr<=4; LedNr++)	// führt Anweisungen mehrmals durch
		{
		led_set(LedNr,1);					// Led auf Zustand 1 setzen
		delay(time);						// Zeitverzögerung um 500ms
		led_set(LedNr,0);					// Led auf Zustand 0 setzen
		delay(time);						// Zeitverzögerung um 500ms
		}
	}
}