/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"


void setup() //setup wird einmal am anfang ausgeführt
{
	led_init(); //leds werden initialisiert
}

void loop() //loop wird ständig wiederholt
{
	led_set(1,1); //led 1 wird eingeschaltet
	delay(500); //0.5ms warten
	led_set(1,0); //led 1 wird ausgeschaltet
	delay(500); //0.5ms warten
}
