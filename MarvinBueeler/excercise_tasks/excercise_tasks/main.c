/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h"

void setup() //Setup wird einmal am Anfang ausgeführt
{
	led_init(); //Leds werden initialisiert
	analog_init(); //alle analogen Datenleitungen (auch die für die Taster) werden initialisiert 
	odometry_init(); //Odometry Sensoren werden initialisiert
	motpwm_init(); //Motoren werden initialisiert
	surface_readPersistent();
}

void loop() //Loop wird ständig wiederholt
{
	analog_wait_update(); //aktualisiert alle Sensorwerte 
	nibo_checkMonitorVoltage(); //prüft Akku spannung. Bei tiefer Spannung weichen Sensorwerte ab
	
	char key = key_get_char(); //key Variabel kann nur in der Loop-Funktion verwendet werden, weil sie hier deklariert wurde
	
	unsigned long int col = surface_getColorRGB(); //Variabel für die Farberkennung
	
	int diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK); //Vergleicht gerade gemessenen Wert mit referenz Wert von Schwarz
	int diff_white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE); //Vergleicht gerade gemessenen Wert mit referenz Wert von Weiss

	/*
	Wenn die Sensoren schwarze Farbe detektieren, leuchtet LED1, bei weisser
	Farbe leuchtet LED2. Sie müssen jedich kalibriert sein.
	*/
	led_set(1, diff_black < 20);
	led_set(2, diff_white < 20);
	
	switch (key)
	{
		case 'a': //Taster 1 kalibriert die Farbe Schwarz
			surface_calibrateBlack();
			surface_writePersistent();
		break;
		
		case 'b': //Taster 2 kalibriert die Farbe weiss
			surface_calibrateWhite();
			surface_writePersistent();
		break;
	}
}
 

/*
unsigned long int ist eine 2 byte Variabel, die nur positiv sein kann.
*/