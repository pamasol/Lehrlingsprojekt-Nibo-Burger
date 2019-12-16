#include <niboburger/robomain.h>
unsigned char state = 0;
int abstand = 20;


void stateMachine(){

	int fll = analog_getValueExt(ANALOG_FLL, 2);

	int fl = analog_getValueExt(ANALOG_FL, 2);

	int frr = analog_getValueExt(ANALOG_FRR, 2);

	int fr = analog_getValueExt(ANALOG_FR, 2);


	switch (state)
	{

		case 1:

		motpid_setSpeed(0,0);
		if (key_get_char() == 'A')
		{
			delay(100);
			state = 2;
			
		}
		break;

		case 2:

		if ((fll<abstand)&&(fl<abstand)&&(frr<abstand)&&(fr<abstand))
		{
			motpid_setSpeed(30,30);
		}

		if (fll>abstand)
		{
			state = 3;
		}

		if (fl>abstand)
		{
			state = 3;
		}

		if (frr>abstand)
		{
			state = 4;
		}

		if (fr>abstand)
		{
			state = 4;
		}

		break;

		case 3:
		motpid_setSpeed(30,-30);
		if ((fll<abstand)&&(fl<abstand))
		{
			state = 2;
		}
		break;

		case 4:
		motpid_setSpeed(-30,30);
		if ((frr<abstand)&&(fr<abstand))
		{
			state = 2;
		}
		break;

	}


}







void setup() {

	led_init();
	motpid_init();
	motpwm_init();
	analog_init();
	odometry_init();
	odometry_reset();
	state = 1;
}



void loop() {

	stateMachine();
	
}