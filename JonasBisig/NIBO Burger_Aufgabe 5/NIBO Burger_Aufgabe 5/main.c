/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int lednr = 1;
	led_set(lednr,1);
	delay(350);
	led_set(lednr,0);
	 lednr = lednr + 1;
	led_set(lednr,1);
	delay(350);
	led_set(lednr,0);
	 lednr = lednr + 1;
	led_set(lednr,1);
	delay(350);
	led_set(lednr,0);
	 lednr = lednr + 1;
	led_set(lednr,1);
	delay(350);
	led_set(lednr,0);
	 lednr = 1;
}