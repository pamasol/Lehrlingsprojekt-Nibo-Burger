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
	
	//surface_readPersistent();

	state = 1;
	
}




void loop() {

	key = key_get_char();
	
	Mitte = surface_get(SURFACE_C);

	Rechts = surface_get(SURFACE_R);

	Links = surface_get(SURFACE_L);
	
	
	
	switch (state)
	
	
	{
		
		case 1:
		
		
		motpid_stop(0);
		
		if (key == 'A')
		
		{
			
			state = 2;
			
		}
		
		
		break;
		
		
		
		case 2:
		
		if (Mitte < 20)
		
		{
			
			motpid_setSpeed(30, 30);
			
		}
		
		else
		
		{
			
			state = 3;
			
		}
		
		break;
		
		
		case 3:
		
		if (Rechts < 200)
		
		{
			
			motpid_setSpeed(30, 10);
		}
		
		if (Mitte < 20)
		
		{
			
			state = 2;
			
		}
		
		if (Links < 200)
		
		{
			
			motpid_setSpeed(10, 30);
			
		}
		
		if (Mitte < 20)
		
		{
			
			state = 2;
			
		}
		
		
	}
	
}




