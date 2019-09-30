/* Includes f�r den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
	led_init();						// Anweisung um Led zu initialisieren
	analog_init();					// Anweisung um Analog Signale zu initialisieren
	odometry_init();				// Anweisung um Odometriesensoren zu initialisieren
}

void loop()
{
	/* Programmcode, den der NIBO Burger immer wieder ausf�hren soll */
	char key = key_get_char();
	
	if (key == 'a')					// Wenn Taster 1 gedr�ckt wurde					
	{
	odometry_resetLeft();			// R�cksetzen des Wertes vom linken Odometriesensor
	}

	if (key == 'b')					// Wenn Taster 2 gedr�ckt wurde
	{
	odometry_resetRight();			// R�cksetzen des Wertes vom rechten Odometriesensor
	}

	if (key == 'c')					// Wenn Taster 3 gedr�ckt wurde
	{
	odometry_reset();				// R�cksetzen des Wertes beider Odometriesensoren
	}

	led_set(1, odometry_getLeft(0)>10);			// Led 1 (rote Led) auf Zustand 1 setzen, wenn der Wert des linken Odometriesensors 10 Umdrehungen gemacht hat
	led_set(2, odometry_getLeft(0)>20);			// Led 2 (blaue Led) auf Zustand 1 setzen, wenn der Wert des linken Odometriesensors 20 Umdrehungen gemacht hat

	led_set(3, odometry_getRight(0)>10);		// Led 3 (blaue Led) auf Zustand 1 setzen, wenn der Wert des rechten Odometriesensors 10 Umdrehungen gemacht hat
	led_set(4, odometry_getRight(0)>20);		// Led 4 (rote Led) auf Zustand 1 setzen, wenn der Wert des rechten Odometriesensors 20 Umdrehungen gemacht hat
	}		