#include "niboburger/robomain.h"

unsigned char state = 1;

int Motorstart;
int OL;
int OR;

void stateMachine()

{

	switch( state )

	{

		case 1:

		motpwm_setLeft(0);

		motpwm_setRight(0);

		odometry_reset();

		OL = 0;

		OR = 0;


		if(key_get_char() == 'A')
		{

			Motorstart = 1;

			state = 2;

		}

		break;

		case 2:

		if(key_get_char() == 'C') state = 1;

		if (Motorstart == 1)

		{

			motpwm_setLeft(500);

			motpwm_setRight(500);

			Motorstart = 0;

		}

		if (odometry_getLeft(0)>5)

		{

			motpwm_setLeft(0);

			OL = 1;

		}



		if (odometry_getRight(0)>5)

		{

			motpwm_setRight(0);

			OR = 1;

		}



		if ((OL == 1)&&(OR == 1))

		{

			OL = 0;

			OR = 0;

			state = 3;

		}

		break;

		case 3:

		if(key_get_char() == 'B')

		{

			motpid_setSpeed(50,50);

		}

		if (odometry_getLeft(0)>1063)

		{

			motpid_setSpeed(0,0);

			odometry_reset();

			delay(100);

			state = 4;

		}

		break;

		case 4:

		motpid_setSpeed(50,-1);



		if (odometry_getLeft(0)>260)

		{

			motpid_setSpeed(0,0);

			odometry_reset();

			delay(100);

			state = 5;

		}

		break;

		case 5:

		motpid_setSpeed(50,50);



		if (odometry_getLeft(0)>1063)

		{

			motpid_setSpeed(0,0);

			odometry_reset();

			delay(100);

			state = 6;

		}

		break;

		case 6:

		motpid_setSpeed(50,-1);



		if (odometry_getLeft(0)>260)
		{

			motpid_setSpeed(0,0);

			odometry_reset();

			delay(100);

			state = 1;

		}

	}

}





void setup()

{

	led_init();

	analog_init();

	motpwm_init();

	motpid_init();

	odometry_init();

}



void loop()

{

	stateMachine();

}