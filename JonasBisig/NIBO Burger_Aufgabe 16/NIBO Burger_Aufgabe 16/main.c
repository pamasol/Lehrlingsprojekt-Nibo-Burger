/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
	analog_init();
	motpwm_init();
}

void test() {
	// das ist eine testfunktion
}

void loop()
{
	analog_wait_update();
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	int l = max(fll , fl);
	int r = max(frr , fr);
	
	if ((l<10) && (r<10))
	{
		motpwm_setLeft(800);
		motpwm_setRight(800);
	} else if (l<r)
	{
		motpwm_setLeft(0);
		motpwm_setRight(800);
	} else if (l>r)
	{
		motpwm_setLeft(800);
		motpwm_setRight(0);
	}
	
}