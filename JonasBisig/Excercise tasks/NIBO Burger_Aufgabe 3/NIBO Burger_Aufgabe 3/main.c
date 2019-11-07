/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int lednr= 1;
	for (lednr=1; lednr<=4; lednr++)
	{
		led_set(lednr,1);
		delay(100);
	}
}