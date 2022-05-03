#include <niboburger/robomain.h>

int instruct=0;
int i=1;
const int left_speed=600; //Max 900!
const int right_speed=600; //Max 900!

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
		
	
	
	if(instruct==1)	{
		

		if(odometry_getLeft(0)>odometry_getRight(0))	{
			rspeed=rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			lspeed=lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			lspeed=left_speed;
			rspeed=right_speed;
		}	

	
		
		switch (i)  {
			case 1:
			motpwm_setLeft(lspeed);
			motpwm_setRight(rspeed);
			if(odometry_getLeft(0)>=1150)	{
			odometry_getLeft(1);
			odometry_getRight(1);
			delay(100);
			i=2;
			}
			break;
		
			case 2:
			while(odometry_getLeft(0)<142)	{
			motpwm_setLeft(400);
			motpwm_setRight(-400);
			}
								
			motpwm_setLeft(0);
			motpwm_setRight(0);
			odometry_getLeft(1);
			odometry_getRight(1);
			delay(100);
			i=3;
			
			break;
			
			case 3:
				motpwm_setLeft(lspeed);
				motpwm_setRight(rspeed);
				if(odometry_getLeft(0)>=1150)	{
				motpwm_setLeft(0);
				motpwm_setRight(0);
				delay(100);
				}
			
			break;
			}
}
	if(instruct==0) {
		i=1;
		motpwm_setLeft(0);
		motpwm_setRight(0);
		odometry_getLeft(1);
		odometry_getRight(1);
	}
	led_set(1, odometry_getLeft(0)>300);
	led_set(2, odometry_getLeft(0)>600);
	
	led_set(3, odometry_getRight(0)>900);
	led_set(4, odometry_getRight(0)>1100);
}