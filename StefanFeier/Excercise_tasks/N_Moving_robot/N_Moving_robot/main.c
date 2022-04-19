#include <niboburger/robomain.h>

int instruct=0;
int left_speed=500;
int right_speed=550;

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
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		instruct=1;
		break;	
		case 'B':
		instruct=0;
	}
		
	analog_wait_update();
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	

	// Checking left area of robot
	int l = max(fll, fl);
	// Checking right area of robot
	int r = max(frr, fr);
	
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
	
	if(instruct==1)	{
	
		if ((l<20) && (r<20)) {
		/*
		No obstacles, move with 80% of full speed.
		*/
		motpwm_setLeft(left_speed);
		motpwm_setRight(right_speed);

			} else if ((l<20) && (r>20)) {
			/*
			Obstacles in right area, turn left.
			*/
			motpwm_setLeft(0);
			motpwm_setRight(500);

			} else if ((l>20) && (r<20)) {
			/*
			Obstacles in left area, turn right.
			*/
			motpwm_setLeft(500);
			motpwm_setRight(0);
		
			} else if ((l>20) && (r>20)) {
			motpwm_setLeft(800);
			motpwm_setRight(-800);
		}
	}
}
