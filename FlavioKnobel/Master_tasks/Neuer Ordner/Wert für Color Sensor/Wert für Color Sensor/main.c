#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>


uint8_t maroon_mode;

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
	uint16_t diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK); //(not used)
	
	
	sprintf(red_val, "%u", diff_red);
	sprintf(green_val, "%u", diff_green);
	sprintf(blue_val, "%u", diff_blue);
	sprintf(yellow_val, "%u", diff_yellow);
	sprintf(white_val, "%u", diff_white);
	sprintf(black_val, "%u", diff_black);
	
	usart_write(MAROON_IMM_CLEAR());
	usart_write(red_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(green_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(blue_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(yellow_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(white_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(black_val);
	delay(5000);
	usart_write("F");
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	delay(5000);
}
