/**
 *	NIBO Burger – Pamasol electronic project for apprentices
 *	Master task D) Colour detection
 *	Setup: maroon shield mounted
 *  Instructions:
 *  1. 
 *  2. 
 *  3. 
 *  Worth knowing:
 *  - 
 */

#include "niboburger/robomain.h"
#include "maroon.h"

uint32_t rgb_color;

void setup() {
    led_init();
    maroon_setup();
    analog_init();
    
	/**	3800 millivolts corresponds to 4.2V battery voltage. The battery
	 *  is fully charged 4.88V (4x1.22V).
     */
	nibo_setMonitorVoltage(3800);
	
    delay(200);

	surface_readPersistent();
	    
    // maroon_welcome();

    

}

void loop() {
    /**	Check if monitor voltage is ok. If below nibo_setMonitorVoltage()
     *  program will be interrupted and LED will blink SOS. This is
     *  important since low voltage will lead to other sensor outputs.
     */
	nibo_checkMonitorVoltage();
	analog_wait_update();
	
	char key = key_get_char();
	
	uint32_t rgb_value = surface_getColorRGB();

	char str[32];
	int num = rgb_value;
	sprintf(str, "0x%06x", num);
	
	switch (key) {
		case 'A':
			usart_write(str);
		break;
	}	
}