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
}

void loop() //Loop wird ständig wiederholt
{
	char key = key_get_char(); //char Variable key deklariert
	
	switch (key) {
		case 'A':
		led_set(1,1);
		led_set(4,1);
		break;
		case 'B':
		led_set(2,1);
		led_set(3,1);
		break;
		case 'C':
		led_setall(0,0,0,0);
		break;
	}
} 