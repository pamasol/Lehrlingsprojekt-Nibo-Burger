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
	int FLL = analog_getValueExt(ANALOG_FLL, 2);	// steuert den Sensor FLL an
	// 0: Wellenlänge 400 nm - 700 nm
	// 1: Wellenlänge 700 nm - 1 mm
	// 2: Gibt den Reflexionswert ohne den sichtbaren Teil zurück.

	int FL = analog_getValueExt(ANALOG_FL, 2);	// steuert den Sensor FL an
	// 0: Wellenlänge 400 nm - 700 nm
	// 1: Wellenlänge 700 nm - 1 mm
	// 2: Gibt den Reflexionswert ohne den sichtbaren Teil zurück.

	int FR = analog_getValueExt(ANALOG_FR, 2);	// steuert den Sensor FR an
	// 0: Wellenlänge 400 nm - 700 nm
	// 1: Wellenlänge 700 nm - 1 mm
	// 2: Gibt den Reflexionswert ohne den sichtbaren Teil zurück.

	int FRR = analog_getValueExt(ANALOG_FRR, 2);	// steuert den Sensor FRR an
	// 0: Wellenlänge 400 nm - 700 nm
	// 1: Wellenlänge 700 nm - 1 mm
	// 2: Gibt den Reflexionswert ohne den sichtbaren Teil zurück.
	
	if (FLL<20)					// Vorne Links Links
	{
		led_set(1,1);
	}

	else
	{
		led_set(1,0);
	}

	if (FL<20)					// Vorne Links
	{
		led_set(2,1);
	}

	else
	{
		led_set(2,0);
	}

	if (FRR<20)					// Vorne Rechts Rechts
	{
		led_set(4,1);
	}
	
	else
	{
		led_set(4,0);
	}

	if (FR<20)					// Vorne Rechts
	{
		led_set(3,1);
	}

	else
	{
		led_set(3,0);
	}
}
