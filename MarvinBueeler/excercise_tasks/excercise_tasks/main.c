/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

int power; //globale Variabel power deklariert, kann in void und loop funktionen verwendet werden

void setup() //Setup wird einmal am Anfang ausgeführt
{
	led_init(); //Leds werden initialisiert
	analog_init(); //alle analogen Datenleitungen (auch die für die Taster) werden initialisiert 
	odometry_init(); //Odometry Sensoren werden initialisiert
	motpwm_init(); //Motoren werden initialisiert
	power = 0; //Beim start wird die Variabel auf 0 gesetzt
}

void loop() //Loop wird ständig wiederholt
{
	char key = key_get_char(); //key Variabel kann nur in der Loop-Funktion verwendet werden, weil sie hier deklariert wurde
	
	switch (key) {
		case 'a':
			power = power + 200; //Taster 1 gedrückt wird 200 zur Variabel power addiert
		break;

		case 'B':
			power = 0; //Taster 2 setzt power wieder auf 0
		break;
		
		case 'c':
			power = power - 200; //Taster 3 subtrahiert 200 von power
		break;
	}
	
	/*
	Motor Geschwindigkeiten liegen zwischen -1024 und 1024. 
	Geschwindigkeit ausserhalb dieses Bereiches werden auf
	-1024 auf- bzw. 1024 abgerundet
	*/
	motpwm_setLeft(power);
	motpwm_setRight(power);
	delay(10); //Motorgeschwindigkeit kann nur alle 10ms geändert werden
} 