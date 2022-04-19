#include <niboburger/robomain.h>

void setup() {
	analog_init();
	led_init();
	motpwm_init();
}

void loop() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations with the same analog
	values is unrewarding.
	*/
	analog_wait_update();
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

	// Checking left area of robot
	int l = max(fll, fl);
	// Checking right area of robot
	int r = max(frr, fr);

	if ((l<20) && (r<20)) {
		/*
		No obstacles, move with 80% of full speed.
		*/
		motpwm_setLeft(800);
		motpwm_setRight(800);

		} else if ((l<20) && (r>20)) {
		/*
		Obstacles in right area, turn left.
		*/
		motpwm_setLeft(0);
		motpwm_setRight(800);

		} else if ((l>20) && (r<20)) {
		/*
		Obstacles in left area, turn right.
		*/
		motpwm_setLeft(800);
		motpwm_setRight(0);
		
		} else if ((l>20) && (r>20)) {
		motpwm_setLeft(800);
		motpwm_setRight(-800);
		}
	
}
