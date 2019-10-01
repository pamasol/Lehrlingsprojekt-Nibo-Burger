/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
/* Programmcode zum einmaligen Setup des Roboters */
{
	led_init();					// Anweisung um Led zu initialisieren
	analog_init();				// Anweisung um Analog Signale zu initialisieren
	motpwm_init();				// Anweisung um Motoren zu initialisieren
	odometry_init();			// Anweisung um Odometriesensoren zu initialisieren
	surface_readPersistent();
}

void loop()
/* Programmcode, den der NIBO Burger immer wieder ausführen soll */
{
	analog_wait_update();
	nibo_checkMonitorVoltage();
	
	unsigned long int col = surface_getColorRGB();
	
	int diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);
	int diff_white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);
	
	led_set(1, diff_black < 20);
	led_set(2, diff_white < 20);
	
	char key = key_get_char();

	if (key == 'a') {
	surface_calibrateBlack();
	surface_writePersistent();
	}
	
	else if (key == 'b')
	{
	surface_calibrateWhite();
	surface_writePersistent();
	}
}
