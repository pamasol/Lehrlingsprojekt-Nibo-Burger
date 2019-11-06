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
	analog_init(); //alle analogen Datenleitungen (auch die f�r die Taster) werden initialisiert 
	odometry_init(); //Odometry Sensoren werden initialisiert
}

void loop() //Loop wird st�ndig wiederholt
{
	int left = odometry_getLeft(0); //Variable f�r Wert von odometry Sensor links
	int right = odometry_getRight(0); //Variable f�r Wert von odometry Sensor rechts
	char key = key_get_char(); //char Variable key deklariert
	
	switch (key) {
		case 'A':
			odometry_resetLeft();
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
