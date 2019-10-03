#include "niboburger/robomain.h"

// Die zwei Leds auf jener Seite wechseln sich ab, wenn die Umdrehungen auf den jeweiligen Seiten erreicht wurden.
// Taster 1: Fahren mit MotPWM -> Roboter fährt nicht geradeaus, da sich einer der beiden Motoren verspätet
// Taster 2: Fahren mit MotPID -> Roboter fährt geradeaus. Eventuell kleine Abweichungen durch den Aufbau des Getriebe.

void setup()
{
	led_init();
	analog_init();		
	motpwm_init();
	motpid_init();
	odometry_init();
}

void loop()
{
	char key = key_get_char();
	switch (key) 
	
	{
		//MotPWM
		case 'A':
		motpwm_setLeft(200);
		motpwm_setRight(200);
		break;
		
		//MotPID
		case 'B':
		motpid_setSpeed(9,9);
		break;
		
		//RESET
		case 'C':
		motpwm_setLeft(0);
		motpwm_setRight(0);
		motpid_setSpeed(0,0);
		break;
	}
	
	if (odometry_getLeft(0)<500)
	{
		led_set(1,1);
		led_set(2,0);
	}
	
	if (odometry_getLeft(0)>500)
	{
		led_set(1,0);
		led_set(2,1);
	}
	
	if (odometry_getLeft(0)>1000)
	{
		odometry_reset();
	}
	
	if (odometry_getRight(0)<500)
	{
		led_set(4,1);
		led_set(3,0);
	}
	
	if (odometry_getRight(0)>500)
	{
		led_set(4,0);
		led_set(3,1);
	}
	
	if (odometry_getRight(0)>1000)
	{
		odometry_reset();
	}
}