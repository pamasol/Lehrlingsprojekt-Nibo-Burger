/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"
int State;
void setup()
{
analog_init();
motpwm_init();
}

void loop()
{
char key = key_get_char();

switch (key)
{
	case 'A':
	State=1;
	break;
	
	case 'B':
	State=0;
	break;
}
if (State==1)
{
motpwm_setLeft(-1024);
motpwm_setRight(1024);
}
}