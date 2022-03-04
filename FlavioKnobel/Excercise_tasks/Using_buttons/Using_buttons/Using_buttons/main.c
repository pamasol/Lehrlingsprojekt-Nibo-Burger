#include "niboburger/robomain.h"

void setup()
{
led_init();
analog_init();
}

void loop()
{
char key = key_get_char();
switch(key)
{
	case 'C':
	led_set(3,1);
	break;
	case 'c':
	led_set(3,0);
	break;

}
}