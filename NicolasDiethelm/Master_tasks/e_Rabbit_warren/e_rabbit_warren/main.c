
#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

uint8_t maroon_mode;

int State=0;
int speedl=0;
int speedr=0;


void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

void setup() {
	led_init();
	motpwm_init();
	analog_init();
	surface_readPersistent();
	maroon_setup();
	surface_init();
	analog_setExtToggleMode(ANALOG_BCL, 1);
	analog_setExtToggleMode(ANALOG_BCR, 1);
}

void loop() {
	
	char Sen[16];
	
	unsigned long int col = surface_getColorRGB();
	
	uint8_t bcl = min(127, surface_get(SURFACE_CL)/8); 
	uint8_t bc  = min(127, surface_get(SURFACE_C)/8);
	uint8_t bcr = min(127, surface_get(SURFACE_CR)/8);
	
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		State=1;
		break;
		case 'B':
		State=0;
		break;
	}
	
	
	
	
	
	if (State==1)
	{
		if (bc<80)
		{
			speedl=-200;
			speedr=-200;
		} 
		if (bcr>=40&&bcr<=50)
		{
			speedr=-300;
			speedl=-200;
		}
		
		if (bcr>50)
		{
			speedr=-500;
			speedl=500;
		}
		if (bcl>=40&&bcl<50)
		{
			speedl=-300;
			speedr=-200;
		}
		
		if (bcl>50)
		{
			speedl=-500;
			speedr=500;
		}
		if (bc>70)
		{
			speedr=0;
			speedl=0;
		}
		
		
	}
	if(State==0)
	{
		speedl=0;
		speedr=0;
	}
	
	motpwm_setLeft(speedl);
	motpwm_setRight(speedr);
}

