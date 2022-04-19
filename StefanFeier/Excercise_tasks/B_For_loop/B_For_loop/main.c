#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int LED;
		
	
	for (LED=1; LED<=4; LED=LED+1)
	{
		led_set(LED, 1);
		delay(500);
		led_set(LED, 0);
	}
}