/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	led_init();				//Anweisung um Led zu initialisieren
}

void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
	led_set(1,1);				//Led 1 (rote LED) auf Zustand 1 setzen
	delay(500);					//Zeitverzögerung um 500ms
	led_set(1,0);				//Led 1 (rote LED) auf Zustand 0 setzen
	delay(500);					//Zeitverzögerung um 500ms
}
