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
	int lednr, time; //neue Variable f�r die Zeit
	
	for(time=50; time<400; time*=2) //Diese for-Schleife erh�ht den Wert der Zeit
	{
		for (lednr=1; lednr<=4; lednr++) //for_schlaufe um die Leds anzusteuern
		{
			led_set(lednr,1);
			delay(time); //time Variable wird f�r die Verz�gerung genutzt
			led_set(lednr,0);
			delay(time);
		}
	}
}
