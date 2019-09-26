#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
}

void loop() {
	/*
	Variables that store analog values of all 4 IR sensors in front of 
	NIBO Burger.
	fll	= Front left of left
	fl	= Front left
	fr	= Front right
	frr	= Front right of right
	*/
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

	// Your code here	
	

}
