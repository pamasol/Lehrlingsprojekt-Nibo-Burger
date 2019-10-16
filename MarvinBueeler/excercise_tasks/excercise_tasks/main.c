/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

int state;
int abstand = 20;
int l;
int r;
int v1;
int v2;

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
	/**********************************
	
	Sensorwerte für Objekterkennung initialisieren
	
	**********************************/
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	
	/**********************************
	
	LED's ansteuern
	
	**********************************/
	
	if (fll > abstand)
	{
		led_set(1,1);
	}else
	{
		led_set(1,0);
	}
	if (fl > abstand)
	{
		led_set(2,1);
	}else
	{
		led_set(2,0);
	}
	if (fr > abstand)
	{
		led_set(3,1);
	}else
	{
		led_set(3,0);
	}
	if (frr > abstand)
	{
		led_set(4,1);
	}else
	{
		led_set(4,0);
	}
	
	l = max(fll, fl);
	r = max(frr, fr);
	
	switch (state)
		{
		case 0:
			 motpid_setSpeed(0,0);
			 led_setall(1,0,0,1);
			 delay(500);
			 led_setall(0,1,1,0);
			 delay(500);
			 
			 if (key_get_char() == 'A')
			 {
				 state = 1;
			 }
		break;
		
		case 1:
				 	
			/**********************************
	
			Geschwindigkeiten
		
			**********************************/
	
			if (r > 40)
			{
				v1 = 50;
			}else if (r > 30)
			{
				v1 = 40;
			}else if (r > abstand)
			{
				v1 = 35;
			}else
			{
				v1 = 0;
			}
	
			if (l > 40)
			{
				v2 = 50;
			}else if (l > 30)
			{
				v2 = 40;
			}else if (l > abstand)
			{
				v2 = 35;
			}else
			{
				v2 = 0;
			}
			motpid_setSpeed(v1,v2);
			
			/**********************************
			
			Ausschaltfunktion

			**********************************/
			
			if (key_get_char() == 'C')
			{
				state = 0;
			}
		break;
		}
}
