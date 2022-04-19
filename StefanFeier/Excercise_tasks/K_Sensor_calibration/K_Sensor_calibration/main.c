#include <niboburger/robomain.h>

void setup() {
	led_init();
	analog_init();
	/*
	Calibration values are stored in the EEPROM and will be 
	persistent when reprogramming. therefore they have to be
	loaded in the setup function.
	*/
	surface_readPersistent();
}

void loop() {
	/*
	Main program should wait until ATmega16 has all sensor values
	updated.
	*/
	analog_wait_update();
	/*
	Checking battery voltage. This is an important step since sensor
	values deviate with low battery voltage.
	*/
	nibo_checkMonitorVoltage();

	/*
	col variable is an unsigned long int that is 2 byte	In this
	scenario, it stores the RGB color value of the surface.	
	*/
	unsigned long int col = surface_getColorRGB();
	
	/*
	Comparing just measured color value with reference value for a black
	and a white surface.
	*/
	int diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);
	int diff_white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);

	/*
	If RGB sensors detect a black surface, LED1 will be switched on, if
	a white surface is detected, LED2 will switch on. Be aware that this 
	function works only when the sensors are calibrated.
	*/
	led_set(1, diff_black < 20);
	led_set(2, diff_white < 20);

	/*
	Putting sensor on a black surface and clicking button 1 calibrates the 
	sensor for the color black. Same procedure can be applied with white
	surface and button 2.
	*/
	char key = key_get_char();
	if (key == 'a') {
		surface_calibrateBlack();
		surface_writePersistent();
		} else if (key == 'b') {
		surface_calibrateWhite();
		surface_writePersistent();
	}
}