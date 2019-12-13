
/* Includes for access to NIBO Burger library */

#include "niboburger/robomain.h"



void setup()

{
	led_init();
}



void loop()

{
	led_set(2, 1);
	delay(500);
	led_set(2, 0);
	delay(500);
}