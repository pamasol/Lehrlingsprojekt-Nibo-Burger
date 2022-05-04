#include "niboburger/robomain.h"
#include "maroon.h"
#include "maroon.c"

void setup() 
{
analog_init();
led_init();
surface_readPersistent();
maroon_setup();
}

void loop() 
{
	
unsigned long col = surface_getColorRGB();
long Red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
long Green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
long Blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
long Yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
long White = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);
long Black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);

if((Red<23000) && (Green>22000) && (Black<20000))	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("RED");
	delay(5000);
}

if((Green<9000) && (Red>29000) && (White>31000) && (Black<20000))	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("GREEN");
	delay(5000);
}

if((Blue<12000) && (Red>29000) && (Black<20000))	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("BLUE");
	delay(5000);
}

if((Yellow<14000) && (Blue>29000) && (Red<20000))	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("YELLOW");
	delay(5000);
}

if((White<32000) && (Black>29000) && (Green<9000) && (Blue>30000) && (Red>30000))	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("WHITE");
	delay(5000);
}


/*			red		green	blue	yellow	white	black
red		=	21088	23124	18517	32767	32767	9216
green	=	32767	7524	26501	32767	32767	15264
blue	=	32767	26987	10310	32767	32767	1736
yellow	=	9000	14057	32767	12153	32767	32077
white	=	32767	7682	32767	21806	30289	32767
black	=	32767	32767	15317	32767	32767	0
nothing	=	32767	32767	15317	32767	32767	0
*/
}
