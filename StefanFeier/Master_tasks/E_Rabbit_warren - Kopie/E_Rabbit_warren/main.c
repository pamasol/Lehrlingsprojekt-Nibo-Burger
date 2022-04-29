#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>

void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}


void setup() {
	
	led_init();
	analog_init();
	//
	//surface_readPersistent();
	//
	surface_init();
	analog_setExtToggleMode(ANALOG_BCL, 1);
	analog_setExtToggleMode(ANALOG_BCR, 1);
	//
	maroon_setup();
	
}

void loop() {
	
	analog_wait_update();
	
	char color_val[8];
	
	uint8_t bcr = min(127, surface_get(SURFACE_CR)/8);

	sprintf(color_val, "%u", bcr);
			
	usart_write(MAROON_IMM_CLEAR());
	usart_write(color_val);
	delay(2000);
	
}