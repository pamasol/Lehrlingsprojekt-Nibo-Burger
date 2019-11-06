/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin B�eler
 */ 

#include "niboburger/robomain.h"


void setup() //Setup wird einmal am Anfang ausgef�hrt
{
	led_init(); //Leds werden initialisiert
}

void loop() //Loop wird st�ndig wiederholt
{
	int lednr;
	
	for (lednr=1; lednr<=4; lednr++)
	{
		led_set(lednr,1);
		delay(500);
		led_set(lednr,0);
		delay(500);
	}
}

//Mit der for-Schlaufe ist der Code viel K�rzer, da ich mit einem Command alle Leds ansteuern kann. Die Funtkion bleibt die gleiche.