#include <niboburger/robomain.h>

int instruct=0;
int left_speed=602;
int right_speed=604;

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
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		instruct=1;
		break;	
		case 'B':
		instruct=0;
	}
		
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
		odometry_getLeft(1);
		odometry_getRight(1);
	}
	
	if(instruct==1)	{
		
		if(odometry_getLeft(0)<1150)	{
			motpwm_setLeft(left_speed);
			motpwm_setRight(right_speed);
			
			if(odometry_getLeft(0)==1150)	{
				motpwm_setLeft(300);
				motpwm_setRight(-300);
				delay(3900);
				motpwm_setLeft(0);
				motpwm_setRight(0);
				odometry_getLeft(1);
				odometry_getRight(1);
				
				if(odometry_getLeft(0)<1150)	{
					motpwm_setLeft(left_speed);
					motpwm_setRight(right_speed);
					
					while(odometry_getLeft(0)>=1150)	{
						motpwm_setLeft(0);
						motpwm_setRight(0);
						break;
						
					}
				}
			}
		}
	}
	led_set(1, odometry_getLeft(0)>1000);
	led_set(2, odometry_getLeft(0)>1100);
	
	led_set(4, odometry_getRight(0)>1000);
	led_set(3, odometry_getRight(0)>1100);

}