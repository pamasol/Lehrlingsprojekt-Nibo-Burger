#include "niboburger/robomain.h"

/************************************************************************/

/* STATEMACHINE                                                         */

/************************************************************************/

unsigned char state = 0;

int Start = 0;
int StatusLED = 1;

int FL = 0;
int FR = 0;


void stateMachine()
{
	if (StatusLED == 1)
	{
		led_setall(1,0,0,1);
		delay(100);
		led_setall(0,0,0,0);
		delay(100);
	}
	
	if(key_get_char() == 'A')
	{
		StatusLED = 0;
		
		led_setall(0,1,1,0);
		delay(1000);

		Start = 1;
	}
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);

	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

	if ((fll>25)&&(Start == 1))
	{
		led_set(1, 1);

		state = 1;
	}

		else
		{
			led_set(1, 0);

		}

	if ((fl>25)&&(Start == 1))
	{
		led_set(2, 1);
		FL = 1;

		state = 1;
	}

		else
		{
			led_set(2, 0);
		}

	if ((fr>25)&&(Start == 1))
	{
		led_set(3, 1);
		FR = 1;

		state = 2;
	}

		else
		{

			led_set(3, 0);
		}

	if ((frr>25)&&(Start == 1))
	{
		led_set(4, 1);

		state = 2;
	}

		else
		{

			led_set(4, 0);
		}

	if((fr>25)&&(fl>25)&&(Start == 1))
	{
		motpid_setSpeed(-50,50);
		delay (500);
		motpid_setSpeed(0,0);
	}


	switch( state )
	{
		case 0:

		if (Start == 1)
		{
			motpid_setSpeed(50,50);
		}

		break;

		case 1:

		if ((fr>25)&&(frr>25))
		{
			motpid_setSpeed(50,-50);
			delay(1000);
			motpid_setSpeed(0,0);

			state = 0;
		}

		if ((fr<25)&&(frr<25))
		{
			motpid_setSpeed(50,-50);
			delay(500);
			motpid_setSpeed(0,0);

			state = 0;
		}

		if ((FR == 1)&&(FL == 1))
		{
			motpid_setSpeed(50,-50);
			delay (500);
			motpid_setSpeed(0,0);

			FR = 0;
			FL = 0;

			state = 0;
		}

		break;

		case 2:

		if ((fl>25)&&(fll>25))
		{
			motpid_setSpeed(-50,50);
			delay(1000);
			motpid_setSpeed(0,0);

			state = 0;
		}

		if ((fl<25)&&(fll<25))
		{
			motpid_setSpeed(-50,50);
			delay(500);
			motpid_setSpeed(0,0);

			state = 0;
		}

		if ((FR == 1)&&(FL == 1))
		{
			motpid_setSpeed(50,-50);
			delay (500);
			motpid_setSpeed(0,0);
			
			FR = 0;
			FL = 0;

			state = 0;
		}

		break;		

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