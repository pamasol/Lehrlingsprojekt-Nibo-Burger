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
			odometry_reset();
			motpwm_setLeft(0);
			motpwm_setRight(0);
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
				odometry_reset();
				state = 3;
			}
			
			break;
			
			case 3:
			if(key_get_char() == 'C') state = 1;
			
			odometry_reset();

			if(key_get_char() == 'B')
			{
				led_setall(0,0,0,0);
				
				if (odometry_getLeft(0)<5)
				{
					led_set(1,1);
				}

				if (odometry_getRight(0)<5)
				{
					led_set(4,1);
				}
				Motorstart = 1;
				state = 4;
				
			}
			break;

			// Räder ausrichten
			case 4:
			if(key_get_char() == 'C') state = 1;

			if (Motorstart == 1)
			{
				motpwm_setLeft(500);
				motpwm_setRight(500);
				Motorstart = 0;
			}

			if (odometry_getLeft(0)>500)
			{
				led_set(1,0);
				led_set(2,1);
				motpwm_setLeft(0);
				endeodometrielinks = 1;
			}
			
			if (odometry_getRight(0)>500)
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
				odometry_reset();
				Motorstart = 1;
				delay(500);
				state = 5;
			}
			
			break;
			
			case 5:
			if(key_get_char() == 'C') state = 1;

			if (Motorstart == 1)
			{
				motpwm_setLeft(500);
				Motorstart = 0;
			}

			if (odometry_getLeft(0)>257)
			{
				led_set(1,0);
				led_set(2,1);
				motpwm_setLeft(0);
				endeodometrielinks = 1;
				odometry_reset();
				Motorstart = 1;
				state = 6;
			}
			
			break;
			
			case 6:
			if(key_get_char() == 'C') state = 1;
			
			odometry_reset();

			if(Motorstart == 1);
			{
				led_setall(0,0,0,0);
				
				if (odometry_getLeft(0)<5)
				{
					led_set(1,1);
				}

				if (odometry_getRight(0)<5)
				{
					led_set(4,1);
				}
				
				state = 7;
				
			}
			break;

			// Räder ausrichten
			case 7:
			if(key_get_char() == 'C') state = 1;

			if (Motorstart == 1)
			{
				motpwm_setLeft(500);
				motpwm_setRight(500);
				Motorstart = 0;
			}

			if (odometry_getLeft(0)>500)
			{
				led_set(1,0);
				led_set(2,1);
				motpwm_setLeft(0);
				endeodometrielinks = 1;
			}
			
			if (odometry_getRight(0)>500)
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
				odometry_reset();

			}
			
			break;
	}	
}


void setup()
{
	led_init();
	analog_init();
	motpwm_init();
	odometry_init();
}

void loop()
{
	stateMachine();
}

