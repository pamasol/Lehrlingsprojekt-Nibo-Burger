#include <niboburger/robomain.h>
#include "maroon.h"

uint8_t maroon_mode;

/** @brief	Setup and enable serial communication with maroon shield
 *			at 38'400 bits per second (38400 baud, 8 bit, no parity,
 *			1 stop bit).
 *
 *  @param  -
 *
 *  @return void
 */
void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
}

/** @brief	Shows "1 Follow me" as horizontal scrolling text on the
 *			8 x 8 pixel LED display which is called maroon shield.
 *
 *  @param  -
 *
 *  @return void
 */
void maroon_welcome() {
	maroon_mode=0;
	usart_write(
		MAROON_IMM_CLEAR()	// Clear display
		MAROON_BRIGHT(0)	// Set matrix brightness to 0%
		MAROON_LOAD()		// Load next matrix bytes without delay
		"1"
		MAROON_BAR("0")		// Bargraph mode, every char is a colon, terminated by space
		MAROON_DIM(*)		// Dimm matrix brightness to 100%
		MAROON_PAUSE(100)	// 100ms pause
		MAROON_DIM(5)		// Dimm matrix brightness to 50%
		MAROON_STIME(80)	// Set shift delay time to 60ms
		" Pamasol Electrics\n"
		MAROON_TXBACK(".")	// Transmit back char (synchronisation) c=[ASCII char]
	);
}


void setup() {
	led_init();
	led_setall(1, 1, 1, 1);
	
	maroon_setup(); 
	delay(200);
	maroon_welcome();
}

void loop() {

}
