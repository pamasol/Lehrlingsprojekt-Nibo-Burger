
#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
}

void loop() {
	
	int value= analog_getValueExt(ANALOG_FR,2);
	
	if (value < 10)
	{
		led_setall(0,0,0,0);
	}else if (value <20)
	{
		led_setall(1,0,0,0);
	}else if (value<30)
	{
		led_setall(1,1,0,0);
	}else if (value<40)
	{
		led_setall(1,1,1,0);
	}else 
	{
		led_setall(1,1,1,1);
	}
}

