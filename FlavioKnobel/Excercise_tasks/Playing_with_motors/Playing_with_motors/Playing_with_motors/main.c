#include <niboburger/robomain.h>

int power;

void setup() {
	led_init();
	analog_init();
	motpwm_init();
	power = 0;
}

void loop() {

	char key = key_get_char();
	
	switch (key) {
		
		case 'a':
			power = power + 200;
		break;

		case 'B':
			power = 0;
		break;
		
		case 'c':
			power = power - 200;
		break;
			
	}
	motpwm_setLeft(power);
	motpwm_setRight(power);

	delay(10);
}