#include <niboburger/robomain.h>

/*
Global variable that is visible everywhere in this program. It
can be accessed for example in void or loop function.
*/
int power;

void setup() {
	led_init();
	analog_init();
	// Initializing of motor control 
	motpwm_init();
	power = 0;
}

void loop() {
	/*
	Local variable key that only is visible in this loop function.
	*/
	char key = key_get_char();
	
	switch (key) {
		/*
		Button 1 released leads to an increased power variable and
		motor speed.
		*/
		case 'a':
			power = power + 200;
		break;
		
		/*
		Button 2 clicked sets power variable to 0 and motor speed
		as well to zero. 
		*/
		case 'B':
			power = 0;
		break;
		/*
		ToDo: Button 3 released leads to a decreased power variable
		and motor speed.
		*/
		
		// Your code here
		
		
	}
	
	/*
	Motor speed method accepts values between -1024 and +1024. Higher values
	are rounded down to 1024, lower values are rounded up to -1024:
	+1024	100 %	Power forward
	+512	50 %	Power forward
	0		0 %		Power
	-512	50 %	Power backward
	-1024	100 %	Power backward
	*/
	motpwm_setLeft(power);
	motpwm_setRight(power);
	
	delay(10);
}


/*
Scope of Variables in C

In general, scope is defined as the extent up to which something
can be worked with. In programming also scope of a variable is
defined as the extent of the program code within which the
variable can we accessed or declared or worked with.
There are mainly two types of variable scopes: global and local.
*/
