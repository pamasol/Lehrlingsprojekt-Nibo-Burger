#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	int ledNr;
	/* For loop starts at LED2, not at LED1 */
	for (ledNr=2; ledNr<=4; ledNr++) {
		led_set(ledNr, 1);
	}
}
