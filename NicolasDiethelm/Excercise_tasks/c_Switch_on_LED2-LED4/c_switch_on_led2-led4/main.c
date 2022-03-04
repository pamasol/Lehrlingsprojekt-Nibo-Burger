
#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	for(int i=2; i<=4; i++)
	{
		led_set(i,1);
		delay(500);
	}
	led_setall(0,0,0,0);
	delay(500);
}

