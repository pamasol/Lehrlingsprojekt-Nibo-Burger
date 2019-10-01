/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
/* Programmcode zum einmaligen Setup des Roboters */
{
	led_init();					// Anweisung um Led zu initialisieren
	analog_init();				// Anweisung um Analog Signale zu initialisieren
	motpwm_init();				// Anweisung um Motoren zu initialisieren
	odometry_init();			// Anweisung um Odometriesensoren zu initialisieren
}

void loop()
/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
{
	int value = analog_getValueExt(ANALOG_FR, 2);	// steuert den Sensor FR an
	// 0: Wellenlänge 400 nm - 700 nm
	// 1: Wellenlänge 700 nm - 1 mm
	// 2: Gibt den Reflexionswert ohne den sichtbaren Teil zurück.
	if (value<10)
	{
		led_set(1,0);
		led_set(2,0);
		led_set(3,0);
		led_set(4,0);
	}
	
	else if (value<20)
	{
		led_set(1,1);
		led_set(2,0);
		led_set(3,0);
		led_set(4,0);
	}
	
	else if (value<30)
	{
		led_set(1,1);
		led_set(2,1);
		led_set(3,0);
		led_set(4,0);
	}
	
	else if (value<40)
	{
		led_set(1,1);
		led_set(2,1);
		led_set(3,1);
		led_set(4,0);
	}
	
	else
	{
		led_set(1,1);
		led_set(2,1);
		led_set(3,1);
		led_set(4,1);
	}
}
