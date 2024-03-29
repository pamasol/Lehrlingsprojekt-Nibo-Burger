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
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
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
		
		
		if (fll>30)	{
			led_set(1,1);
			i=1;
		}
		else
		{
			
		}
		
		if (fl>30)	{
			led_set(2,1);
			i=2;
		}
		else
		{
			
		}
		
		if (fr>30)	{
			led_set(3,1);
			i=3;
		}
		else
		{
			
		}
		
		if (frr>30)	{
			led_set(4,1);
			i=4;
		}
		else
		{
			
		}
		
		if ((fll>30) && (fl>30) && (fr>30) && (frr>30))	{
			i=5;
		}
		else
		{
			
		}
		
		
		if ((fll>30) && (frr>30) && (fl<30) && (fr<30))	{
			i=6;
		}
		else
		{
			
		}
		
		if ((fl>30) && (fr>30) && (fll<30) && (frr<30))	{
			i=6;
		}
		else
		{
			
		}
		
		if (fll<20)	{
			led_set(1,0);
		}
		else
		{
			
		}
		
		if (fl<20)	{
			led_set(2,0);
		}
		else
		{
			
		}
		
		if (fr<20)	{
			led_set(3,0);
		}
		else
		{
			
		}
		
		if (frr<20)	{
			led_set(4,0);
		}
		else
		{
			
		}
		
		switch (i)  {
			case 1:	//obstacle on the far left side
			motpwm_setLeft(400);
			motpwm_setRight(-400);
			delay(600);
			i=0;
			
			break;
		
		
			case 2:	//obstacle on the left side
			motpwm_setLeft(400);
			motpwm_setRight(-400);
			delay(600);
			i=0;
						
			break;
			
			
			case 3:	//obstacle on the right side
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(600);
			i=0;			
				
			break;
			
			
			case 4:	//obstacle on the far right side
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(600);
			i=0;
						
			break;
			
			
			case 5:	//corner
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(700);
			i=0;
			
			break;
			
			
			case 6:	//corner
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(1500);
			i=0;
			
			break;
			
			
			default:	//no obstacle
			motpwm_setLeft(left_speed);
			motpwm_setRight(right_speed);
						
			break;
			}
}
	if(instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
}