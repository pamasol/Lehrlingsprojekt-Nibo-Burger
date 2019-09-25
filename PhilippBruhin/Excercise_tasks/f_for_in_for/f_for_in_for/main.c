#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	int time, ledNr;
	/*
	This loop increases the time variable from 50 to 100 to 200 ms.
	*/	
	for (time=50; time<400; time*=2) {
		
		/*
		This loop switches on and off every led from 1 to 4. The time
		is given by the outer loop.
		*/
		for (ledNr=1; ledNr<=4; ledNr++) {
			led_set(ledNr, 1);
			delay(time);
			led_set(ledNr, 0);
			delay(time);
		}

	}
}
