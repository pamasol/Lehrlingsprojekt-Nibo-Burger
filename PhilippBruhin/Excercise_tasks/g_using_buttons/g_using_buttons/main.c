#include <niboburger/robomain.h>

void setup() {
	led_init();

	/*
	Analog init initializes all analog data lines, including buttons.
	One always has to add this line if buttons are used in the program.
	*/
	analog_init();
}

void loop() {

	/*
	Char can take one character. The character has to be placed in between
	two quotes.
	*/
	char key = key_get_char();

	/*
	The switch statement allows a case differentiation. Depending on the
	value of the key variable a specific case is processed.
	*/
	switch (key) {
		case 'A':
			led_set(1,1);
		break;
		case 'a':
			led_set(1,0);
		break;
		case 'B':
			led_set(2,1);
		break;
		case 'b':
			led_set(2,0);
		break;
		case 'C':
			led_set(3,1);
		break;
		case 'c':
			led_set(3,0);
		break;
	}
}
