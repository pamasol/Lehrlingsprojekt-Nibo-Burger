#include <niboburger/robomain.h>
#include "maroon.h"

/** @brief  Setup and enable serial communication with maroon shield
 *          at 38'400 bits per second (38400 baud, 8 bit, no parity,
 *          1 stop bit).
 *
 *  @param  -
 *
 *  @return void
 */
void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
}

/** @brief  Shows "1 Follow me" as horizontal scrolling text on the
 *          8 x 8 pixel LED display which is called maroon shield.
 *
 *  @param  -
 *
 *  @return void
 */
void maroon_welcome() {
	usart_write(
		MAROON_IMM_CLEAR()              // Clear display
		MAROON_BRIGHT(*)                // Set matrix brightness to 100%
		MAROON_LOAD()                   // Load next matrix bytes without delay
		"1"
		MAROON_BAR("0")                 // Bargraph mode, every char is a colon, terminated by space
                                        // Number one from line 33 is shifted 1 pixel to the left.
                                        // Write "00" if you want 2 pixels.
		MAROON_PAUSE(1000)              // 1000ms pause
		MAROON_STIME(3000)              // Set shift delay time to 3000ms
		" Pamasol\n"
        MAROON_LOAD()                   // Load next matrix bytes without delay (in this case
                                        // load swiss cross without scrolling).
        MAROON_GFX("ffe7e78181e7e7ff")  // Bargraph mode. Hex number of two chars represent 8 bits
                                        // ff = 11111111
                                        // e7 = 11100111
                                        // e7 = 11100111
                                        // 81 = 10000001
                                        // 81 = 10000001
                                        // e7 = 11100111
                                        // e7 = 11100111
                                        // ff = 11111111
		MAROON_TXBACK(".")              // Transmit back char (synchronization)
	);
}


void setup() {
	led_init();
	led_setall(1, 1, 1, 1);
	delay(200);

	maroon_setup(); 
	delay(200);
	maroon_welcome();
}

void loop() {

}
