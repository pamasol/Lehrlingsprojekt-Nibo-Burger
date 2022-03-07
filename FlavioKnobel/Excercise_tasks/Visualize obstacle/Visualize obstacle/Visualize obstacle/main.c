#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
}

void loop() {
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

		if (fll<20)
		{
			led_set(1,0);
		} 
		else
		{
			led_set(1,1);
		}
		if (fl<20)
		{
			led_set(2,0);
		}
		else
		{
			led_set(2,1);
		}
		if (fr<20)
		{
			led_set(3,0);
		}
		else
		{
			led_set(3,1);
		}
		if (frr<20)
		{
			led_set(4,0);
		}
		else
		{
			led_set(4,1);
		}
	

}