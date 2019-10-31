/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup() {
	led_init();
}

void loop() {
	int time, ledNr;
	for (time=50; time<400; time*=2) {
		for (ledNr=1; ledNr<=4; ledNr++) {
			led_set(ledNr, 1);
			delay(time);
			led_set(ledNr, 0);
			delay(time);
		}
	}
}