#include "niboburger/robomain.h"
int ON;
int MotorSpeedL;
int MotorSpeedR;
void setup()
{
motpid_init();
analog_init();
led_init();
odometry_init();
}

void loop()
{
char key = key_get_char();
switch(key)
{
case'A':
ON=1;
break;
}
if (odometry_getLeft(0)>odometry_getRight(0))
{
MotorSpeedR=1024;
}
if (odometry_getLeft(0)<odometry_getRight(0))
{
MotorSpeedL=1024;
}
if (odometry_getLeft(0)==odometry_getRight(0))
{
MotorSpeedL=950; 
MotorSpeedR=950;
}
switch (ON){
case 1:
	odometry_getLeft(1);
	odometry_getRight(1);
	ON=2;
	delay(100);
	break;
case 2:
	motpwm_setRight(MotorSpeedR);
	motpwm_setLeft(MotorSpeedL);
	if 	(odometry_getRight(0)>200)
	{
	motpwm_setRight(0);
	motpwm_setLeft(0);
	delay(1000);
	ON=3;
	}
break;
case 3:
	odometry_getLeft(1);
	odometry_getRight(1);
	ON=4;
	delay(100);
break;
case 4:
	motpwm_setRight(-500);
	motpwm_setLeft(500);
	if 	(odometry_getLeft(0)>23)
	{			
	motpwm_setRight(0);
	motpwm_setLeft(0);
	delay(1000);
	ON=5;		
	}
break;
case 5:
	odometry_getLeft(1);
	odometry_getRight(1);
	delay(100);
	ON=6;
break;
case 6:
	motpwm_setRight(MotorSpeedR);
	motpwm_setLeft(MotorSpeedL);
	if 	(odometry_getRight(0)>200)
	{
	motpwm_setRight(0);
	motpwm_setLeft(0);
	delay(1000);
	ON=7;
	}
break;
case 7:
	odometry_getLeft(1);
	odometry_getRight(1);
	delay(100);
	ON=8;
break;
case 8:
motpwm_setRight(-500);
motpwm_setLeft(500);
if 	(odometry_getLeft(0)>23)
{
	motpwm_setRight(0);
	motpwm_setLeft(0);
	ON=20;
}
break;
}
}


