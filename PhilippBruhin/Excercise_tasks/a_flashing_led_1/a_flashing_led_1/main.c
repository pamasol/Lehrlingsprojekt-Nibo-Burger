/* Includes for access to NIBO Burger library */
#include "niboburger/robomain.h"

void setup()
{
	/* Code that is executed once at startup */
	led_init();
}

void loop()
{
	/* Code that is repeated infinitely on NIBO Burger */
	led_set(1, 1);
	delay(500);
	led_set(1, 0);
	delay(500);
}