/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"
int drehen = 130;
int gerade = 100;
int led;
int los = 0;
void setup()
{
	motpwm_init();
	odometry_init();
	odometry_reset();
	analog_init();
	led_init();
}

void loop()
{
	odometry_reset();
	//Pro Tick bewegt sich der Roboter 1,41 mm 
	
	char key = key_get_char();
	
	//led_setall(0,1,1,0);

	switch (key)
	{
		case 'a':
		los = 1;
		break;
	}
		if (los == 1)
		{
			delay(1000);
			motpwm_setLeft(1000);
			motpwm_setRight(1000);
			los = 2;
		}
		
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= gerade && los == 2)
		{
			motpwm_setRight(0);
			motpwm_setLeft(0);
			odometry_reset();
			los = 3;
		}
		if (los == 3)
		{
			delay(1000);
			motpwm_setLeft(500);
			motpwm_setRight(-500);
			los = 4;
		}
			
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= drehen && los == 4)
		{	
			motpwm_setRight(0);
			motpwm_setLeft(0);
			odometry_reset();
			los = 5;
		}

		if (los == 5)
		{
			delay(1000);
			motpwm_setRight(1000);
			motpwm_setLeft(1000);
			los = 6;
		}
				
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= gerade && los == 6)
		{
			motpwm_setRight(0);
			motpwm_setLeft(0);
			odometry_reset();
			los = 7;
		}
		if (los == 7)
		{
			delay(500);
			motpwm_setLeft(500);
			motpwm_setRight(-500);
			los = 8;
		}
		
		if (max(odometry_getLeft(0), odometry_getRight(0)) >= drehen && los == 8)
		{
			motpwm_setRight(0);
			motpwm_setLeft(0);
			odometry_reset();
			los = 9;
		}
		
		if (los == 9)
		{
			
		for (led = 1; led <=4; led++)
		{
		led_set(led, 1);
		delay(500);
		}
		delay(500);
		led_setall(0,0,0,0);
		}
}