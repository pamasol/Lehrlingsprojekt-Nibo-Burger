/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

int odometrie_kali;
int runde;
int zaehler;
int starterlaubnis;
int drehen;

void odometrie_kalibrieren()
{
	odometry_reset();
	do
	{
		if (odometry_getLeft(0)<1)
		{
			led_setall(1,0,0,1);
			motpid_setSpeed(10,10);
		} else if (odometry_getLeft(0)>1)
		{
			led_setall(0,1,1,0);
			motpid_stop(1);
			odometrie_kali = 0;
			starterlaubnis = 1;
			odometry_reset();
			loop();
		}
	} while (odometrie_kali == 1);
}

void rundedrehen()
{
	do
	{
		if (odometry_getLeft(0)<1083)
		{
			motpid_setSpeed(50,50);
		}
		if (odometry_getLeft(0)>1083)
		{
			odometry_reset();
			drehen = 1;
			do 
			{
				if (odometry_getLeft(0)<130)
				{
					motpid_setSpeed(50,-50);
				}
				if (odometry_getLeft(0)>130)
				{
					motpid_setSpeed(0,0);
					drehen = 0;
				}
			} while (drehen ==1);
			zaehler = zaehler + 1;
			odometry_reset();
		}
		if (zaehler==2)
		{
			motpid_stop(1);
			runde = 0;
			starterlaubnis = 0;
			zaehler = 0;
			led_setall(1,0,0,1);
			odometry_reset();
			loop();
		}
	} while (runde == 1);
}

void setup() //Setup wird einmal am Anfang ausgeführt
{
	led_init(); //Leds werden initialisiert
	analog_init(); //alle analogen Datenleitungen (auch die für die Taster) werden initialisiert 
	odometry_init(); //Odometry Sensoren werden initialisiert
	motpid_init(); //Motoren werden initialisiert
	surface_readPersistent(); //Laden der Kalibrierung aus EEPROM
	odometry_reset();
	led_setall(1,0,0,1);
	odometrie_kali = 0;
	runde = 0;
	zaehler = 0;
	starterlaubnis = 0;
	drehen = 0;
}

void loop() 
{
	char key = key_get_char();
	
	switch (key)
	{
		/*
		Odometrie kalibrieren
		*/
		case 'a':
			if(starterlaubnis == 0)
				{
					odometrie_kali = 1;
					odometrie_kalibrieren();
				}
		break;
		
		case 'b':
			if(starterlaubnis == 1)
				{
					runde = 1;
					rundedrehen();
				}
		break;
			
	}
}
