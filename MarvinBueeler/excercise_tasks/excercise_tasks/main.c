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
}

void loop() //Loop wird ständig wiederholt
{
	int left = odometry_getLeft(0); //Variable für Wert von odometry Sensor links
	int right = odometry_getRight(0); //Variable für Wert von odometry Sensor rechts
	char key = key_get_char(); //char Variable key deklariert
	
	switch (key) {
		case 'A':
			odometry_resetLeft();
			led_set(1,1);
			led_set(1,0);
			break;
		case 'B':
			odometry_resetRight();
			break;
		case 'C':
			odometry_reset();
			break;
	}
	
	led_set(1,left>10);
	led_set(2,left>20);
	led_set(3,right>20);
	led_set(4,right>10);
} 