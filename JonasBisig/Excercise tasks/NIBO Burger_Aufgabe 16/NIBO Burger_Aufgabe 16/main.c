/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

// Globale Variabeln
int status;

void setup()
{
	led_init();
	analog_init();
	motpwm_init();
	
	
}

void loop()
{
	int key;
	
	analog_wait_update();

	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	int l = max(fll , fl);
	int r = max(frr , fr);
	
	switch (key)
	{
		case 'a':
		status = 1;
	}
	
	if ((l<20) && (r<20))
	{
		motpwm_setLeft(800);
		motpwm_setRight(800);
		led_setall(1,0,0,1);
	} else if (l<r)
	{
		motpwm_setLeft(0);
		motpwm_setRight(800);
		led_setall(0,0,1,0);
	} else if (l>r)
	{
		motpwm_setLeft(800);
		motpwm_setRight(0);
		led_setall(0,1,0,0);
	}
	
}