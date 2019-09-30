/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	led_init();				// Anweisung um Led zu initialisieren
}

void loop()
{
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