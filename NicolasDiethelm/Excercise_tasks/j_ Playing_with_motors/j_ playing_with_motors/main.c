
#include <niboburger/robomain.h>

int power = 0; 

void setup() {
	led_init();
	analog_init();
	motpwm_init();
}

void loop() {
	
	char key= key_get_char();
	
	switch (key)
	{
		case 'A':
		power=power-200;
		break;
		
		case 'B':
		power=0;
		break;
		
		case 'C':
		power = power + 200;
	}
	
	motpwm_setLeft(power);
	motpwm_setRight(power);
	
}

