#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	led_set(1, 1);
	delay(500);
	led_set(1, 0);
	delay(500);
	led_set(2, 1);
	delay(500);
	led_set(2, 0);
	delay(500);
	led_set(3, 1);
	delay(500);
	led_set(3, 0);
	delay(500);
	led_set(4, 1);
	delay(500);
	led_set(4, 0);
	delay(500);
}