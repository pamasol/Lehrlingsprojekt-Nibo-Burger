#include "niboburger/robomain.h"

/************************************************************************/
/* STATEMACHINE                                                         */
/************************************************************************/




void stateMachine()
{
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	if (fll>22)
	{
		led_set(1, 1);
		motpid_setSpeed(0,50);
	}
	else
	{
		led_set(1, 0);
	}

	if (fl>22)
	{
		led_set(2, 1);
		motpid_setSpeed(0,50);
	}
	else
	{
		led_set(2, 0);
	}

	if (fr>22)
	{
		led_set(3, 1);
		motpid_setSpeed(50,0);
	}
	else
	{
		led_set(3, 0);
	}

	if (frr>22)
	{
		led_set(4, 1);
		motpid_setSpeed(50,0);
	}
	else
	{
		led_set(4, 0);
	}
	
	if ((fr>22)&&(fl>22))
	{
		motpid_setSpeed(50,50);
	}
	
	if ((fll<22)&&(fl<22)&&(fr<22)&&(frr<22))
	{
		motpid_setSpeed(0,0);
		led_setall(1,0,0,1);
		delay(100);
		led_setall(0,0,0,0);
		delay(100);
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
		
	
	
