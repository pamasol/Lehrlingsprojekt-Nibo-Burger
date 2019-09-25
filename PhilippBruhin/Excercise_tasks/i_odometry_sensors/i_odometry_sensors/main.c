#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
	odometry_init();
}

void loop() {
	char key = key_get_char();
	
	// Reset odometry left
	if (key == 'a') {
		odometry_resetLeft();
	}
	
	// Reset odometry right
	if (key == 'b') {
		odometry_resetRight();
	}
	
	// Reset odometry left and right
	if (key == 'c') {
		odometry_reset();
	}
	
	// Testing of left odometry sensor
	led_set(1, odometry_getLeft(0)>10);
	led_set(2, odometry_getLeft(0)>20);
	
	// Testing of right odometry sensor

	// Your code here
	
	
	
}
