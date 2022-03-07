#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
}

void loop() {
	
	/*
	Function returns the value of IR sensor in slot FL. ANALOG_FL
	is a constant. Second parameter:
	0 =	Returns value based on human visible spectrum (Wavelength 400 nm - 700 nm)
	1 =	Returns value based on human visible spectrum together with 
		the IR spectrum (700 nm - 1 mm)
	2 =	Returns the reflection value of the IR spectrum, without the visible part.
	*/
	int value = analog_getValueExt(ANALOG_FL, 2);
	
	/*
	Switch off all LEDs if the reflection is less than 10 and switch on all LEDs
	when reflection is more than 40.
	*/
	if (value<10) {
		led_setall(0,0,0,0);
		} else if (value<20) {
		led_setall(1,0,0,0);
		} else if (value<30) {
		led_setall(1,1,0,0);
		} else if (value<40) {
		led_setall(1,1,1,0);
		} else {
		led_setall(1,1,1,1);
	}
}