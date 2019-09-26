#include <niboburger/robomain.h>

void setup() {
	analog_init();
	led_init();
	motpwm_init();
}

void loop() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations the same	analog
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

	if ((l<10) && (r<10)) {
		/*
		No obstacles, move with 80% of full speed.
		*/
		motpwm_setLeft(800);
		motpwm_setRight(800);

		} else if (l<r) {
		/*
		Obstacles in right area, turn left.
		*/
		motpwm_setLeft(0);
		motpwm_setRight(800);

		} else if (l>r) {
		/*
		Obstacles in left area, turn right.
		*/
		motpwm_setLeft(800);
		motpwm_setRight(0);
	}

}
