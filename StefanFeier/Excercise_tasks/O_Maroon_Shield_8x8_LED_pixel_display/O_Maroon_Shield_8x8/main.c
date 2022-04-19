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
void setup() {
	
	maroon_setup();
	

}

void loop() {
	
	usart_write(MAROON_BRIGHT(0) MAROON_LOAD() MAROON_GFX("cccc3333cccc3333") MAROON_DIM(*) MAROON_PAUSE(800) MAROON_DIM(0));
	delay(2400);
	usart_write(MAROON_BRIGHT(0) MAROON_LOAD() MAROON_GFX("3333cccc3333cccc") MAROON_DIM(*) MAROON_PAUSE(800) MAROON_DIM(0));
	delay(2400);
}