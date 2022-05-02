#include "niboburger/robomain.h"
#include "maroon.h"
#include <stdio.h>
char red_val[16];
char green_val[16];
char blue_val[16];
char yellow_val[16];
char white_val[16];
char black_val[16];

void setup()
{
	analog_init();
	led_init();
	surface_readPersistent();
	maroon_setup();
}

void maroon_setup()
{
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

void loop()
{
unsigned long int col = surface_getColorRGB();
uint16_t red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
uint16_t green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
uint16_t blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
uint16_t yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
uint16_t white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);
uint16_t black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);

sprintf(red_val, "%u", red);
sprintf(green_val, "%u", green);
sprintf(blue_val, "%u", blue);
sprintf(yellow_val, "%u", yellow);
sprintf(white_val, "%u", white);
sprintf(black_val, "%u", black);

	usart_write(MAROON_IMM_CLEAR());
	usart_write(red_val);
	delay(5000);
}