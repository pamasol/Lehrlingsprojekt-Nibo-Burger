
#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	for (int wait=50; wait<=200; wait*=2)
	{
	
	for(int i=1; i<=4; i++)
	{
		led_set(i,1);
		delay(wait);
		led_set(i,0);
		delay(wait);
	}
	}
}

