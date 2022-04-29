#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

int instruct=0;
int i=1;
const int left_speed=600; //Max 900!
const int right_speed=600; //Max 900!

void setup() {
	analog_init();
	led_init();
	motpwm_init();
	odometry_init();
	surface_init();
	analog_setExtToggleMode(ANALOG_BCL, 1);
	analog_setExtToggleMode(ANALOG_BCR, 1);
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

void loop() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations with the same analog
	values is unrewarding.
	*/
	analog_wait_update();
	
	int lspeed=left_speed;
	int rspeed=right_speed;
	
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		instruct=1;
		break;	
		case 'B':
		instruct=0;
	}
		
	
	
	if(instruct==1)	{
		
		char bcr_val[8];
		
		uint8_t bcr = min(127, surface_get(SURFACE_CR)/8); // Green light on right transistor
		uint8_t bcl = min(127, surface_get(SURFACE_CL)/8); // Green light on left transistor
		uint8_t bl  = min(127, surface_get(SURFACE_L) /8);
		uint8_t bc  = min(127, surface_get(SURFACE_C) /8);
		uint8_t br  = min(127, surface_get(SURFACE_R) /8);
		
		
		sprintf(bcr_val, "%x", bcr);
		
		
		if(odometry_getLeft(0)>odometry_getRight(0))	{
			rspeed=rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			lspeed=lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			lspeed=left_speed;
			rspeed=right_speed;
		}
		
		
		if (bc>=50)	{
			led_set(2,1);
			led_set(3,1);
			//i=1;
		}
		else
		{
			
		}
		
		if (bcl>=1)	{
			led_set(1,1);
			//i=2;
		}
		else
		{
			
		}
		
		if (bcr>=1)	{
			led_set(4,1);
			//i=3;
		}
		else
		{
			
		}
		
		if (bc<=50)	{
			led_set(2,0);
			led_set(3,0);
			//i=1;
		}
		else
		{
			
		}
		
		if (bcl<=1)	{
			led_set(1,0);
			//i=2;
		}
		else
		{
			
		}
		
		if (bcr<=1)	{
			led_set(4,0);
			//i=3;
		}
		else
		{
			
		}
		
		usart_write(MAROON_IMM_CLEAR());
		usart_write(bcr_val);
		delay(2000);
		
	
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
}
}