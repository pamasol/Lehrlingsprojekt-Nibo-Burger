
#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

uint8_t maroon_mode;

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
		Stop=0;
		Turn=0;
		State=1;
		break;
		case 'B':
		State=4;
		break;
		case 'C':
		State=2;
		break;
	}
	
	if (State==2)
	{
		odometry_reset();
		speedl=-1023;
		speedr=-1023;
		State=3;
	}
	

	if (State==3)
	{
		
		if (odometry_getRight(0)==-30)
		{
			speedr=0;
		}
		if (odometry_getLeft(0)==-30)
		{
			speedl=0;
		}
		
	}
	
	
	if (State==1)
	{
		speedl=-1023;
		speedr=-1023;
		if (odometry_getRight(0)<=-242)
		{
			speedl=0;
			speedr=0;
			Turn=1;
			State=0;
		}
	}
	if (Turn==1)
	{
		speedl=-1023;
		speedr=1023;
		if (odometry_getLeft(0)<=-272)
		{
			Stop=1;
			Turn=0;
		}
	}
	
	
	if (Stop==1)
	{
		speedl=-1023;
		speedr=-1023;
			if (odometry_getRight(0)<=-464)
			{
				speedl=0;
				speedr=0;
				Stop=4;
			}
	}
	if (State==4)
	{
		speedr=0;
		speedl=0;
	}
	
	motpwm_setLeft(speedl);
	motpwm_setRight(speedr);
}

