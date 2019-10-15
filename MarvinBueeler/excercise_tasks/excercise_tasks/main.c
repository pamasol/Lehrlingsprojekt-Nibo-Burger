/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

unsigned char state;
int abstand = 30;

/**********************************

Statemachine

**********************************/

void statemachine()
{
	
	/**********************************
	
	Sensorwerte für Objekterkennung initialisieren
	
	**********************************/
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	
	/**********************************
	
	LED's über Sensoren steuern
	
	**********************************/
	
	if (fll > abstand)
	{
		led_set(1,1);
	} else
	{
		led_set(1,0);
	}
	
	if (fl > abstand)
	{
		led_set(2,1);
	} else
	{
		led_set(2,0);
	}
	
	if (fr > abstand)
	{
		led_set(3,1);
	} else
	{
		led_set(3,0);
	}
	
	if (frr > abstand)
	{
		led_set(4,1);
	} else
	{
		led_set(4,0);
	}
	
	
	switch(state)
	{
		case 1:
		
			/**********************************
			
			Anzeigen dass er im State 1 Modus ist
			
			**********************************/
			
			motpid_setSpeed(0,0);
			led_setall(1,0,0,0);
			delay(200);
			led_setall(0,1,0,0);
			delay(200);
			led_setall(0,0,1,0);
			delay(200);
			led_setall(0,0,0,1);
			delay(200);
			
			/**********************************
			
			Auf Taster 1 warten, damit Start signalisiert werden kann
			
			**********************************/
			
			if (key_get_char() == 'A')
			{
				
				led_setall(1,0,1,0);
				delay(500);
				led_setall(0,1,0,1);
				delay(500);
				led_setall(0,0,0,0);
				state = 2;
				
			}
			
		break;
		
		case 2:
			
			/**********************************
			
			Vorwaerts fahren bis ein Sensor anspricht
			
			**********************************/
			if ((fll < abstand) && (fl < abstand) && (fr < abstand) && (frr < abstand))
			{
				
				motpid_setSpeed(50,50);
				
			}
			
			if ((fl) > abstand)
			{
				
				state = 3;
				
			}
		
			if ((fll) > abstand)
			{
				
				state = 3;
				
			}
			
			if ((frr) > abstand)
			{
				
				state = 4;
				
			}
			
			if ((fr) > abstand)
			{
				
				state = 4;
				
			}
			
			/**********************************
			
			Taster C ist der Ausschalttaster
			
			**********************************/
			
			if (key_get_char() == 'C')
			{
				
				motpid_setSpeed(0,0);
				led_setall(1,0,0,1);
				delay(1000);
				led_setall(0,0,0,0);
				state = 1;
				
			}
					
		break;
		
		/**********************************
		
		Rechtsmanoever
		
		**********************************/
		
		case 3:
			
			motpid_setSpeed(50, -50);
			
			if((fll < abstand) && (fl < abstand))
			{
				
				if ((frr) > abstand)
				{
					
					state = 5;
					
				}
				
				state = 2;
				
			}
			
		break;
			
		/**********************************
		
		Linksmanoever 
		
		**********************************/
		
		case 4:
			
			motpid_setSpeed(-50, 50);
			
			if ((frr < abstand) && (fr < abstand))
			{
				
				if ((fll) > abstand)
				{
					
					state = 6;
					
				}
				
				state = 2;
				
			}
			
		break;
		
		/**********************************
		
		Rechtsmanoever bei Erkennung von Wand rechts
		bei Rechtsmanoever
		
		**********************************/
		
		case 5:
			
			motpid_setSpeed(50, -50);
			
			if ((frr < abstand) && (fr < abstand))
			{
				
				state = 3;
				
			}
			
		break;
		
		/**********************************
		
		Linksmanoever bei Erkennung von Wand links
		bei linksmanoever
		
		**********************************/
		
		case 6:
		
			motpid_setSpeed(-50, 50);
			
			if ((fll < abstand) && (fl < abstand))
			{
				
				state = 4;
				
			}
			
		break;
			
	}
}

/**********************************

Setup wird beim Start durchgeführt, hier werden Sensoren usw. initialisiert

**********************************/

void setup() 
{
	led_init(); 
	analog_init(); 
	odometry_init(); 
	motpid_init(); 
	surface_readPersistent(); 
	odometry_reset();
	state = 1;
}

void loop() 
{
	statemachine();
}
