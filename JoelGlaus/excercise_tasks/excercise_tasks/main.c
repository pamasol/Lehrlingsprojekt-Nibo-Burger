#include "niboburger/robomain.h"

int endekalibrierunglinks;
int endekalibrierungrechts;
int startkalibrierung;

void kalibrierung()
{
	
	motpwm_setLeft(200);
	motpwm_setRight(200);
	
	if (odometry_getLeft(0)>5)
	{
		led_set(1,0);
		led_set(2,1);
		motpwm_setLeft(0);
		endekalibrierunglinks = 1;
	}
	
	if (odometry_getRight(0)>5)
	{
		led_set(4,0);
		led_set(3,1);
		motpwm_setRight(0);
		endekalibrierungrechts = 1;
	}
	
	if ((endekalibrierunglinks = 1)&&(endekalibrierungrechts = 1))
	{
		odometry_reset();
		loop();
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
	
char key = key_get_char();

switch (key)
	{
	case 'A':
	led_setall(1,0,0,1);
	startkalibrierung = 1;
	kalibrierung();
	break;
	}
}

