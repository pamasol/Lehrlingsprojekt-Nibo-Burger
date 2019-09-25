#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	int ledNr;
	/*
	For loop that does the same process with all LEDs. It does
	not matter if there are 4 or 400, the programming effort
	is the same. Furthermore this code looks cleaner than
	without for statement.
	*/
	for (ledNr=1; ledNr<=4; ledNr++) {
		led_set(ledNr, 1);
		delay(500);
		led_set(ledNr, 0);
		delay(500);
	}
}