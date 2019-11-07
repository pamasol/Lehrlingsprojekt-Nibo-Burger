#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int ledNr;
	/* For loop starts at LED2, not at LED1 */
	for (ledNr=1; ledNr<=4; ledNr++) {
		led_set(ledNr, 1);
		delay(100);
	}
	led_setall(0,0,0,0);
	delay(100);
}
