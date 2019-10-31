/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup() {
	led_init();
	analog_init();
}

void loop() {
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	led_set(1, fll >= 20);
	led_set(2, fl >= 20);
	led_set(3, fr >= 20);
	led_set(4, frr >= 20);
}