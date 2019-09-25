#include "niboburger/robomain.h"

void setup()
{
	led_init();
}

void loop()
{
	/*
	Define an integer variable named ledNr. This variable is uninitialized
	because we haven't given it a value.
	*/
	int ledNr;
	
	/* For loop */
	for (ledNr=1; ledNr<=4; ledNr++) {
		
		/* Variable assignment*/
		led_set(ledNr, 1);
	}
}

/*
About variables and objects in C

Initialization = The object is given an known value at the point of definition.
Assignment = The object is given a known value beyond the point of definition.
Uninitialized = The object has not been given a known value yet.
*/
