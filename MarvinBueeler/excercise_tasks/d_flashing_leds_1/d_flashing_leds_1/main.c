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
	led_set(1,1);
	delay(500);
	led_set(1,0);
	delay(500);
	led_set(2,1);
	delay(500);
	led_set(2,0);
	delay(500);
	led_set(3,1);
	delay(500);
	led_set(3,0);
	delay(500);
	led_set(4,1);
	delay(500);
	led_set(4,0);
	delay(500);
}