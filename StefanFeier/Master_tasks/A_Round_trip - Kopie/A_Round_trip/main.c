#include <niboburger/robomain.h>

int instruct=0;
const int left_speed=600;
const int right_speed=600;

void setup() {
	analog_init();
	led_init();
	motpwm_init();
	odometry_init();
}

void loop() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations with the same analog
	values is unrewarding.
	*/
	
	
	int lspeed=left_speed;
	int rspeed=right_speed;
	
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		instruct=1;
		break;	
		case 'B':
		instruct=0;
	}
	end:	
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
		odometry_getLeft(1);
		odometry_getRight(1);
	}
	
	if(instruct==1)	{
		
		while(odometry_getLeft(0)<133)	{
		motpwm_setLeft(400);
		motpwm_setRight(-400);
		
		}
		instruct=0;
	}
	led_set(1, odometry_getLeft(0)>100);
	led_set(2, odometry_getLeft(0)>200);
	
	led_set(4, odometry_getRight(0)>100);
	led_set(3, odometry_getRight(0)>200);
}