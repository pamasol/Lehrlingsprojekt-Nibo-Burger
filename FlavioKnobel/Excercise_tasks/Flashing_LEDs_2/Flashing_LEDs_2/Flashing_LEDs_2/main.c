#include "niboburger/robomain.h"

void setup()
{
led_init();
}

void loop()
{
int LedNr;

for(LedNr=1; LedNr<=4; LedNr++)
{
	led_set(LedNr,1);
	delay(500);
	led_set(LedNr,0);
	delay(500);
}
}