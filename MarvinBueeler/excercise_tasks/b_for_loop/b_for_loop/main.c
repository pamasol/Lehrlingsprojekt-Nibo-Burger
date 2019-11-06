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
	int lednr; //Variable mit dem Namen "lednr" deklariert
	
	for (lednr=1; lednr<=4; lednr++) //for-Schleife um Led 1-4 einzuschalten
	{
		led_set(lednr, 1);
		delay(1000); //Verzögerung eingefügt, dass Prozess besser erkennt wird
	}
}

//for Schleife führt Anweisung mehrfach aus. Sie hat 3 Parameter: Initialisierung, Bedingungsprüfung, Aktualisierung
