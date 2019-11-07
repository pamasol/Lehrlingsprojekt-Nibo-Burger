/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"


void setup()
{
	motpid_init();
	odometry_init();
	odometry_reset();
	analog_init();
}

void loop()
{
	//Pro Tick bewegt sich der Roboter 1,41 mm 
	
	char key = key_get_char();
		
	switch (key)
	{
		case 'a':
		
		delay(1000);
		motpid_setSpeed(100,100);
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= 1083)
		{
			motpid_stop(0);
			odometry_reset();
		}
		delay(1000);
		motpid_setSpeed(50,-50);
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= 130)
		{
			motpid_stop(1);
			odometry_reset();
		}
		delay(1000);
		motpid_setSpeed(100,100);
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= 1083)
		{
			motpid_stop(0);
			odometry_reset();
		}
		delay(500);
		motpid_setSpeed(50,-50);
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= 130)
		{
			motpid_stop(1);
			odometry_reset();
		}
	}
}