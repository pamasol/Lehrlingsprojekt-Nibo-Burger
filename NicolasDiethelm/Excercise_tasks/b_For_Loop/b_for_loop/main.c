
#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	for(int i=1; i<=4; i++)
	{
		led_set(i,1);
		delay(500);
	}
}

