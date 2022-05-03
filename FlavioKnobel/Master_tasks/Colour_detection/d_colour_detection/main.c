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

if(Red<36000)	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("RED");
	delay(5000);
}

if(Green<33000)	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("GREEN");
	delay(5000);
}

if(Blue<20000)	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("BLUE");
	delay(5000);
}

if(Yellow<33000)	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("YELLOW");
	delay(5000);
}

if(White<33000)	{
	usart_write(MAROON_IMM_CLEAR());
	usart_write("WHITE");
	delay(5000);
}

if((Red>=39000) && (Green>=36000) && (Blue>22000) && (Yellow>=34000) && (White>=34000))	{
	usart_write(MAROON_IMM_CLEAR());
	delay(500);
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
