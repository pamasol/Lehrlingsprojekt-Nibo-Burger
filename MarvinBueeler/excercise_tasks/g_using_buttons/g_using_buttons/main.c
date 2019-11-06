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
	
	switch (key) { //ermöglicht verschiedene Fallunterscheidungen
		case 'A': //bestimmter Fall(csae) von der char Variable hier: 'A' 
		led_set(1,1); //Anweisung bei diesem case hier: Led 1 einschalten
		break; //beendet jeweiligen case und switch-Anweisung
		case 'a':
		led_set(1,0);
		break;
		case 'B':
		led_set(2,1);
		break;
		case 'b':
		led_set(2,0);
		break;
		case 'C':
		led_set(3,1);
		break;
		case 'c':
		led_set(3,0);
		break;
	}
}

//char Variablen speichern immer ein Buchstabe oder ein Zeichen ab. 
