/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
	analog_init();
}

void loop()
{
	char key=key_get_char();
	
	switch (key)
	{
		case 'A':
		led_set(1,1);
		break;
		case 'a':
		led_set(1,0);
		break;
		case 'B':
		led_set(2,1);
		break;
		case 'b':
		led_set(2,0);
		break;
		case 'C':
		led_set(3,1);
		break;
		case 'c':
		led_set(3,0);
		break;
	}
}