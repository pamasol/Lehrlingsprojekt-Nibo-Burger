/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h" 

int state;
int abstand = 20;
int m;
int r;
int l;
int key;
int speed1 = 30;
int speed2 = 20;
int speed3 = -20;

void setup() 
{

	led_init(); 
	analog_init(); 
	odometry_init(); 
	motpid_init(); 
	surface_readPersistent(); 
	odometry_reset();
	state = 0;
}

void loop() 
{	
	key = key_get_char();
	m = surface_get(SURFACE_C);
	r = surface_get(SURFACE_R);
	l = surface_get(SURFACE_L);
	
	
	if (m < 20)
	{
		led_set(1,1);
		led_set(4,1);
	} else
	{
		led_set(1,0);
		led_set(4,0);
	}
	if (l < 200)
	{
		led_set(2,1);
	} else
	{
		led_set(2,0);
	}
	if (r < 200)
	{
		led_set(3,1);
	} else
	{
		led_set(3,0);
	}
	
	switch (state)
	{
		case 0:
		
			motpid_setSpeed(0,0);
			
			if (key == 'A')
			{
				state = 1;
			}
			
		break;
		
		case 1:
		
			motpid_setSpeed(speed1,speed1);
			
			if (m > 20)
			{
				state = 2;
			}
			
			if (key == 'B')
			{
				state = 0;
			}
			
		break;
		
		case 2:
			
			if (l < 300)
			{
				motpid_setSpeed(speed3,speed2);
			}
			if (r < 200)
			{
				motpid_setSpeed(speed2,speed3);
			}
			if (m < 20)
			{
				motpid_setSpeed(0,0);
				state = 1;
			}
			
			if (key == 'B')
			{
				state = 0;
			}
			
		break;
	}


}