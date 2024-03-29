#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

int instruct=0;
int i=0;
const int left_speed=400; //Max 900!
const int right_speed=400; //Max 900!

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
		
		
		//sprintf(bcr_val, "%u", bc);
		
		
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
		
		
		if(bc<=65)	{
			led_set(2,1);
			led_set(3,1);
		}
		
		if(bcl<=90)	{
			led_set(1,1);
		}
		
		if(bcr<=65)	{
			led_set(4,1);
		}
		
		
		if(bc>65)	{
			led_set(2,0);
			led_set(3,0);
		}
		
		if(bcl>90)	{
			led_set(1,0);
		}
		
		if(bcr>65)	{
			led_set(4,0);
		}
		
		
		if((bc<=65) && (bcl<=90) && (bcr<=65))	{
			i=1;
		}
		
		if((bc<=65) && (bcl<=90) && (bcr>=65))	{
			i=2;
		}
		
		if((bc<=65) && (bcl>=90) && (bcr<=65))	{
			i=3;
		}
		
		if((bc>=65) && (bcl<=90) && (bcr>=65))	{
			i=4;
		}
		
		if((bc>=65) && (bcl>=90) && (bcr<=65))	{
			i=5;
		}
		
		if((bc>=65) && (bcl>=90) && (bcr>=65))	{
			i=6;
		}
		
		
		if(i==1)	{
			motpwm_setLeft(left_speed);
			motpwm_setRight(right_speed);
			delay(10);
		}
		
		if(i==2)	{
			motpwm_setLeft(left_speed-1200);
			motpwm_setRight(right_speed+600);
			delay(10);
		}
		
		if(i==3)	{
			motpwm_setLeft(left_speed+600);
			motpwm_setRight(right_speed-1200);
			delay(10);
		}
		
		if(i==4)	{
			motpwm_setLeft(-800);
			motpwm_setRight(right_speed+1000);
			delay(10);
		}
		
		if(i==5)	{
			motpwm_setLeft(left_speed+1000);
			motpwm_setRight(-800);
			delay(10);
		}
		
		if(i==6)	{
			motpwm_setLeft(0);
			motpwm_setRight(0);
			delay(80);
		}
		
	}
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
		i=0;
	}

	
}
