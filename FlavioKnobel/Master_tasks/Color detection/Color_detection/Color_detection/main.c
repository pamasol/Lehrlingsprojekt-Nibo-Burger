#include "niboburger/robomain.h"
#include "maroon.h"

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

if(red<)
{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("RED");
	delay(1000);
}

if(green<)
{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("GREEN");
	delay(1000);
}

if(blue<)
{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("BLUE");
	delay(1000);
}

if(yellow<)
{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("YELLOW");
	delay(1000);
}

if(white<)
{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("WHITE");
	delay(1000);
}

if((red>=) && (green>=) && (blue>=) && (yellow>=) && (white>=))
{
	usart_write(MAROON_IMM_CLEAR());
	delay(1000);
}
}