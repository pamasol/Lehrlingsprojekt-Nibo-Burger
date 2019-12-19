#include <niboburger/robomain.h>


int state;

int Mitte;

int Rechts;

int Links;

int key;



void setup() {

	led_init();

	motpid_init();

	motpwm_init();

	analog_init();

	odometry_init();

	odometry_reset();
	
	surface_readPersistent();

	state = 1;
	
}







void loop() {

	led_setall(1, 1, 1, 1);
	
}




