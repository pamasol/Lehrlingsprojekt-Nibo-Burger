#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
	odometry_init();
}

void loop() {
	char key = key_get_char();

	if (key == 'a') {
		odometry_resetLeft();
	}
	if (key == 'b') {
		odometry_resetRight();
	}
	if (key == 'c') {
		odometry_reset();
	}
	
	led_set(1, odometry_getLeft(0)>10);
	led_set(2, odometry_getLeft(0)>20);
	
	led_set(4, odometry_getRight(0)>10);
	led_set(3, odometry_getRight(0)>20);
			
}