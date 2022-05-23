#include "niboburger/robomain.h"
#include "maroon.h"
int ON;
int gerade;
int Hupe;
void setup()
{
	motpid_init();
	analog_init();
	led_init();
	odometry_init();
	usart_setbaudrate(38400);
	usart_enable();
}
void maroon_vorwaerz() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("103070ffff703010")

	);
}
void maroon_E() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("030D31CDCD310D03")

	);
}


void maroon_left() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("18181818ff7e3c18")

	);
}
void loop()
{
	char key = key_get_char();
	switch(key)
	{
	case 'A':
	maroon_E();
	gerade=1;
	break;	
	
	case'B':
	if (gerade==10){
	ON=1;
	}
	break;
	
	case 'C':
	Hupe=0;
	motpwm_setRight(0);
	break;
	}
	
	
	switch (ON){
		case 1:
		odometry_reset();
		ON=2;
		delay(100);
		maroon_vorwaerz();
		break;
		
		case 2:
		if (odometry_getLeft(0)<odometry_getRight(0))
		{
		motpwm_setRight(-1024);
		}
		if (odometry_getLeft(0)>odometry_getRight(0))
		{
		motpwm_setLeft(-1024);
		}
		if (odometry_getLeft(0)==odometry_getRight(0))
		{
		motpwm_setLeft(-900);
		motpwm_setRight(-900);
		}
		if 	(odometry_getRight(0)<-200)
		{
		motpwm_setRight(0);
		motpwm_setLeft(0);
		delay(1000);
		ON=3;
		}
		break;
		
		case 3:
		odometry_reset();
		ON=4;
		delay(100);
		maroon_left();
		break;
		
		case 4:
		motpwm_setRight(-500);
		motpwm_setLeft(500);
		if 	(odometry_getLeft(0)>27)
		{
		motpwm_setRight(0);
		motpwm_setLeft(0);
		delay(1000);
		ON=5;
		}
		break;
		
		case 5:
		odometry_reset();
		delay(100);
		maroon_vorwaerz();
		ON=6;
		break;
		
		case 6:
		if (odometry_getLeft(0)<odometry_getRight(0))
		{
		motpwm_setRight(-1024);
		}
		if (odometry_getLeft(0)>odometry_getRight(0))
		{
		motpwm_setLeft(-1024);
		}
		if (odometry_getLeft(0)==odometry_getRight(0))
		{
		motpwm_setLeft(-900);
		motpwm_setRight(-900);
		}
		if 	(odometry_getRight(0)<-200)
		{
		motpwm_setRight(0);
		motpwm_setLeft(0);
		delay(1000);
		ON=7;
		}
		break;
		case 7:
		odometry_reset();
		delay(100);
		maroon_left();
		ON=8;
		break;
		
		case 8:
		motpwm_setRight(-500);
		motpwm_setLeft(500);
		if 	(odometry_getLeft(0)>27)
		{
		motpwm_setRight(0);
		motpwm_setLeft(0);
		ON=9;
		}
		break;
		
		case 9:
		odometry_reset();
		usart_write(MAROON_IMM_CLEAR());
		usart_write("Finish");
		delay(5000);
		Hupe=2;
		ON=20;
		break;
	}
	
		switch (gerade){
			case 1:
			motpwm_setLeft(450);
			motpwm_setRight(450);
			if (odometry_getLeft(0)>20)
			{
			motpwm_setLeft(0);
			}
			if (odometry_getRight(0)>20)
			{
			motpwm_setRight(0);
			}
			if (odometry_getRight(0)>20 && odometry_getLeft(0)>20)
			{
			gerade=10;
			}
			delay(100);
			break;
		}
		
		if(Hupe==2)
		{
		motpwm_setRight(1024);
		delay(1);
		motpwm_setRight(-1024);
		delay(1);
		}
}