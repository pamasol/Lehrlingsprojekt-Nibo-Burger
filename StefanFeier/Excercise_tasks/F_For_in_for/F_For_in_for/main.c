#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int LED;
	int timer;
	
	
	for (timer=50; timer<=400; timer=timer*=2)
	{
		for (LED=1; LED<=4; LED=LED+1)
		{
			led_set(LED, 1);
			delay(timer);
			led_set(LED, 0);
		}
	}
}