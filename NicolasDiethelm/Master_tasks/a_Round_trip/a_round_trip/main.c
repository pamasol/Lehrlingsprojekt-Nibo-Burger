
#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

uint8_t maroon_mode;

int Task=0;
int Stop=0;
int Turn=0;
int State=0;
int speedr=0;
int speedl=0;



void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}



void setup() {
	 led_init();
	 analog_init();
	 odometry_init();
	 motpwm_init();
	 
	 
}

void loop() {
	
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		odometry_reset();
		speedl=-300;
		speedr=-300;
		Task=2;
		break;
		case 'B':
		Task=0;
		speedl=0;
		speedr=0;
		break;
		case 'C':
		break;
	}
	
	if (Task==2)
	{
		if (odometry_getLeft(0)<=-212)
		{
			speedl=-300;
			speedr=300;
			Task=3;
		}
	}
	
	if (Task==3)
	{
		
		if (odometry_getLeft(0)<=-246)
		{
			odometry_reset();
			Task=4;
			
		}
	}
	
	if (Task==4)
	{
		speedl=-300;
		speedr=-300;
		if (odometry_getRight(0)<=-212)
		{
			speedl=0;
			speedr=0;
			odometry_reset();
			Task=5;
		}
	}
	
	if (Task==5)
	{
		speedl=-300;
		speedr=300;
		if (odometry_getLeft(0)<=-34)
		{
			speedl=0;
			speedr=0;
			Task=0;
		}
		
	}
	
	if ((odometry_getLeft(0)<=-30)&&(Task==1))
	{
		speedl=0;
	}
	
	if ((odometry_getRight(0)<=-30)&&(Task==1))
	{
		speedr=0;
	}
	
	if ((odometry_getLeft(0)==odometry_getRight(0))&&((Task==2)||(Task==4)))
	{
		speedl=-300;
		speedr=-300;
	}
	
	if ((odometry_getLeft(0)>odometry_getRight(0))&&((Task==2)||(Task==4)))
	{
		speedl=-900;
		speedr=-300;
	}
	
	if ((odometry_getRight(0)>odometry_getLeft(0))&&((Task==2)||(Task==4)))
	{
		speedr=-900;
		speedl=-300;
	}
	motpwm_setLeft(speedl);
	motpwm_setRight(speedr);
}

