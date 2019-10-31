/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	led_init();
	analog_init();
	motpwm_init();
}

void loop()
{
	char key = key_get_char();
	
	switch (key)
	{
		case 'a':
		motpwm_setLeft(800);
		motpwm_setRight(800);
		break;
		case 'B':
		motpwm_setRight(0);
		motpwm_setLeft(0);
		break;		
		case 'c':
		motpwm_setRight(-800);
		motpwm_setLeft(-800);
		break;
	}
}