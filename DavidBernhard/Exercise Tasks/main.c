#include <niboburger/robomain.h>

int state;

int V1;

int V2;






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


	int fll = analog_getValueExt(ANALOG_FLL, 2);

	int fl = analog_getValueExt(ANALOG_FL, 2);

	int frr = analog_getValueExt(ANALOG_FRR, 2);

	int fr = analog_getValueExt(ANALOG_FR, 2);


	int Rechts = max(frr, fr);
	
	int Links = max(fll, fl);
	
	

	switch (state)
	
	{
		case 1:

		motpid_setSpeed(10,10);

		if (key_get_char() == 'A')

		{

			delay(100);

			state = 2;

		}

		break;



		case 2:

		if (Rechts > 40)

		{

			V1 = 50;

		}

		else if (Rechts > 30)

		{

			V1 = 40;

		}

		else if (Rechts > 20)

		{

			V1 = 30;

		}

		else
		
		{
			V1 = 0;
		}
		
		
		if (Links > 40)

		{

			V2 = 50;

		}

		else if (Links > 30)

		{

			V2 = 40;

		}

		else if (Links > 20)

		{

			V2 = 30;

		}

		else

		{
			V2 = 0;
		}
		
		

		motpid_setSpeed(V1, V2);
		
		


		break;
		
		
	}

	}