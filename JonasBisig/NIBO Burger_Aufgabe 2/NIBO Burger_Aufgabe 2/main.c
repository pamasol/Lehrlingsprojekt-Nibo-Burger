/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	led_set(2,1);
	led_set(3,1);
	led_set(1,0);
	led_set(4,0);
	delay(500);
	led_set(2,0);
	led_set(3,0);
	led_set(4,1);
	led_set(1,1);
	delay(500);
}