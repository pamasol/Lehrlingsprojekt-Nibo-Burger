/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

void setup() //Setup wird einmal am Anfang ausgeführt
{
	led_init(); //Leds werden initialisiert
	analog_init(); //alle analogen Datenleitungen (auch die für die Taster) werden initialisiert 
	odometry_init(); //Odometry Sensoren werden initialisiert
	motpwm_init(); //Motoren werden initialisiert
	surface_readPersistent(); //Laden der Kalibrierung aus EEPROM
}

void loop() {
	/*
	Variables that store analog values of all 4 IR sensors in front of 
	NIBO Burger.
	fll	= Front left of left
	fl	= Front left
	fr	= Front right
	frr	= Front right of right
	*/
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

	// Your code here
	
	if (fll>20) //vorne links links
	{
		led_set(1,1);
	} else 
	{
		led_set(1,0);	
	}
	
	if (fl>20) //vorne links
	{
		led_set(2,1);
	} else
	{
		led_set(2,0);
	}
	
	if (fr>20) //vorne rechts
	{
		led_set(3,1);
	} else
	{
		led_set(3,0);
	}
	
	if (frr>20) //vorne rechts rechts
	{
		led_set(4,1);
	} else
	{
		led_set(4,0);
	}
	
	

}