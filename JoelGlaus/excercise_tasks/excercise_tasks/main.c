#include "niboburger/robomain.h"

/************************************************************************/
/* STATEMACHINE                                                         */
/************************************************************************/

unsigned char state = 1;
int Motorstart;
int endeodometrielinks;
int endeodometrierechts;

void stateMachine()
{
	switch( state )
	{

		// Warten bis jemand Taster 1 drückt
		case 1:
		led_setall(0,0,0,0);
		motpwm_setLeft(0);
		motpwm_setRight(0);
		odometry_reset();
		endeodometrielinks = 0;
		endeodometrierechts = 0;
		
		if(key_get_char() == 'A')
		{
			if (odometry_getLeft(0)<5)
			{
				led_set(1,1);
			}

			if (odometry_getRight(0)<5)
			{
				led_set(4,1);
			}
			Motorstart = 1;
			state = 2;
		}
		break;

		// Räder ausrichten
		case 2:
		if(key_get_char() == 'C') state = 1;

		if (Motorstart == 1)
		{
			motpwm_setLeft(500);
			motpwm_setRight(500);
			Motorstart = 0;
		}

		if (odometry_getLeft(0)>5)
		{
			led_set(1,0);
			led_set(2,1);
			motpwm_setLeft(0);
			endeodometrielinks = 1;
		}
		
		if (odometry_getRight(0)>5)
		{
			led_set(4,0);
			led_set(3,1);
			motpwm_setRight(0);
			endeodometrierechts = 1;
		}
		
		if ((endeodometrielinks == 1)&&(endeodometrierechts == 1))
		{
			endeodometrielinks = 0;
			endeodometrierechts = 0;
			state = 3;
		}
		
		break;
		
		// Warten bis jemand Taster 2 drückt -> 1.5m vorwärts fahren
		case 3:
		if(key_get_char() == 'B')
		{
			led_setall(0,0,0,0);
			led_setall(1,0,0,1);
			motpid_setSpeed(50,50);
		}
		
		if (odometry_getLeft(0)>1063)
		{
			motpid_setSpeed(0,0);
			led_setall(0,1,1,0);
			odometry_reset();
			delay(1000);
			state = 4;
		}	
			
		 break;
		
		//180 Grad Linkskurve
		case 4:
		led_setall(0,0,0,0);
		led_setall(1,0,0,1);
		motpid_setSpeed(50,-1);
		
		if (odometry_getLeft(0)>260)
		{
			motpid_setSpeed(0,0);
			led_setall(0,1,1,0);
			odometry_reset();
			delay(1000);
			state = 5;
		}
		
		break;
		
		//1.5m vorwärts fahren
		case 5:
		led_setall(0,0,0,0);
		led_setall(1,0,0,1);
		motpid_setSpeed(50,50);
		
		if (odometry_getLeft(0)>1063)
		{
			motpid_setSpeed(0,0);
			led_setall(0,1,1,0);
			odometry_reset();
			delay(1000);
			state = 6;
		}
		
		break;
		
		//180 Grad Linkskurve
		case 6:
		led_setall(0,0,0,0);
		led_setall(1,0,0,1);
		motpid_setSpeed(50,-1);
		
		if (odometry_getLeft(0)>260)
		{
			motpid_setSpeed(0,0);
			led_setall(0,1,1,0);
			odometry_reset();
			delay(1000);
			state = 1;
		}
		
	}
}


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
	stateMachine();
}