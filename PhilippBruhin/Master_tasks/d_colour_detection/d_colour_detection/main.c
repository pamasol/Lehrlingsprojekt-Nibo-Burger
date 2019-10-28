/**
 *	NIBO Burger – Pamasol electronic project for apprentices
 *	Master task D) Colour detection
 *	Setup: maroon shield mounted
 */

#include "niboburger/robomain.h"
#include "maroon.h"

int inByte = 0;


void setup() {

    led_init();
    maroon_setup();
    analog_init();

    maroon_welcome();
    

}

void loop() {
    maroon_loop();
}