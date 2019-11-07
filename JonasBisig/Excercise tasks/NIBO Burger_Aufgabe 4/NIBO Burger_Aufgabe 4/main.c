/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup() {
	led_init();
}

void loop() {
	int ledNr=4;
	for (ledNr=4; ledNr>=1; ledNr--) {
		led_set(ledNr, 1);
		delay(100);
		led_set(ledNr, 0);
	}
}
