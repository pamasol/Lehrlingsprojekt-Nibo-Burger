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
}

void loop() //Loop wird ständig wiederholt
{
	int lednr, time; //neue Variable für die Zeit
	
	for(time=50; time<400; time*=2) //Diese for-Schleife erhöht den Wert der Zeit
	{
		for (lednr=1; lednr<=4; lednr++) //for_schlaufe um die Leds anzusteuern
		{
			led_set(lednr,1);
			delay(time); //time Variable wird für die Verzögerung genutzt
			led_set(lednr,0);
			delay(time);
		}
	}
}