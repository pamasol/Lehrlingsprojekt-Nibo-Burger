
#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>


uint8_t maroon_mode;

int State=0;

void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

void setup() {
	
	led_init();
	analog_init();
	surface_readPersistent();
	maroon_setup();
	
}

void loop() {
	
	char key = key_get_char();
	
	
	switch (key) {
		case 'A':
		State=1;
		break;
		case 'B':
		State=0;
		break;
		
	}
	
	char red_val[16];
	char green_val[16];
	char blue_val[16];
	char yellow_val[16];
	char white_val[16];
	char black_val[16];
	
	unsigned long int col = surface_getColorRGB();
	
	uint16_t diff_red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
	uint16_t diff_green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
	uint16_t diff_blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
	uint16_t diff_yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
	uint16_t diff_white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);
	
	sprintf(red_val, "%u", diff_red);
	sprintf(green_val, "%u", diff_green);
	sprintf(blue_val, "%u", diff_blue);
	sprintf(yellow_val, "%u", diff_yellow);
	sprintf(white_val, "%u", diff_white);
	
	
	if (State==1)
	{
		if(diff_red<4000)	{
			usart_write(MAROON_IMM_CLEAR());
			usart_write("RED");
			delay(5000);
		}
		
		if(diff_green<10000)	{
			usart_write(MAROON_IMM_CLEAR());
			usart_write("GREEN");
			delay(5000);
		}
		
		if(diff_blue<10000)	{
			usart_write(MAROON_IMM_CLEAR());
			usart_write("BLUE");
			delay(5000);
		}
		
		if(diff_yellow<5000)	{
			usart_write(MAROON_IMM_CLEAR());
			usart_write("YELLOW");
			delay(5000);
		}
		
		if(diff_white<6200)	{
			usart_write(MAROON_IMM_CLEAR());
			usart_write("WHITE");
			delay(5000);
		}

		if((diff_red>=32000) && (diff_green>=32000) && (diff_blue>14000) && (diff_yellow>=32000) && (diff_white>=32000))	{
			usart_write(MAROON_IMM_CLEAR());
			delay(500);
		}
		
	}else{
		usart_write(MAROON_IMM_CLEAR());
		delay(1000);
	}
	
	
}
