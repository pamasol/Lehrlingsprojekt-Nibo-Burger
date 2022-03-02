/**
 *  NIBO Burger – Pamasol electronic project for apprentices
 *  Master task D) Colour detection
 *  Setup: maroon shield mounted
 *  Instructions:
 *      1. Switch on robot and place it on colored surface.
 *      2. Click key 3 to get the color as hex value.
 *  Worth knowing:
 *      This program makes heavily use of RGB colors. R represents
 *      the red value, G the green and B the blue. Examples:
 *	    Red     #ff0000 --> 1111 1111 0000 0000 0000 0000
 *      Green   #00ff00 --> 0000 0000 1111 1111 0000 0000
 *      Blue    #0000ff --> 0000 0000 0000 0000 1111 1111
 *      Yellow  #ffff00 --> 1111 1111 1111 1111 0000 0000
 *      White   #ffffff --> 1111 1111 1111 1111 1111 1111
 *      Black   #000000 --> 0000 0000 0000 0000 0000 0000 
 */

#include <stdlib.h>
#include <string.h>

#include "niboburger/robomain.h"
#include "maroon.h"

enum {
	EVENT_NONE              =  0,
	EVENT_KEY1              =  1,
	EVENT_KEY2              =  2,
	EVENT_KEY3              =  3,
};

enum {
	COL_NONE,
	COL_BLACK,
	COL_WHITE,
	COL_RED,
	COL_BLUE,
	COL_GREEN,
	COL_YELLOW
};

char rgb_str[] = "#000000";

/** @brief  Makes an LED blink with 80ms on and 120ms off
 *
 *  @param  led     LED number (1,2,3 or 4)
 *  @param  count   How many times led should blink (1 up to 32767)
 *
 *  @return void
 */
void blink_led(uint8_t led, uint8_t count) {
    while (count--) {
        led_set(led, 1);
        delay(80);
        led_set(led, 0);
        delay(120);
    }
}

/** @brief  Takes binary number and transforms it to hex number.
 *          Updates global variable rgb_str[] in a second step.
 *
 *  @param  val     Binary number with 8 bit
 *  @param  pos     Position in the hex string (first of two from left)
 *
 *  @return void
 */
void transform_to_hex (uint8_t val, uint8_t pos) {
	char c1=val/16;
	char c2=val%16;
	if (c1<10) c1+='0'; else c1+='a'-10;
	if (c2<10) c2+='0'; else c2+='a'-10;
	rgb_str[pos] = c1;
	rgb_str[pos+1] = c2;
}

/** @brief  Takes 24 bit binary RGB value and divides it into binary
 *          data for red, green and blue.
 *
 *  @param  rgb     RGB color value 24 bit as 32 bit unsigned int
 *
 *  @return void
 */
void rgb_color_to_string (uint32_t rgb) {
	uint8_t r = (uint8_t)((rgb >> 16) & 0xff);
	uint8_t g = (uint8_t)((rgb >>  8) & 0xff);
	uint8_t b = (uint8_t)((rgb >>  0) & 0xff);
	transform_to_hex(r, 1);
	transform_to_hex(g, 3);
	transform_to_hex(b, 5);
}

/************************************************************************/
/* EVENT HANDLING                                                       */
/************************************************************************/

/** @brief  Returns which button is clicked or EVENT_NONE
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 */
uint8_t key_getEvent() {
    static uint8_t key = 0;
    uint8_t act = key_get_state();
    if (act==0) {
        act = key;
        key = 0;
        if (act==0x01) return EVENT_KEY1;
        if (act==0x02) return EVENT_KEY2;
        if (act==0x04) return EVENT_KEY3;
        return EVENT_NONE;
    }
    if (act!=KEY_STATE_INVALID) {
        key |= act;
    }
    return EVENT_NONE;
}

/** @brief  Checks if a button is clicked and if so, returns it as event.
 *          If no button is clicked it returns event none.
 *          If no obstacle event it returns event none.
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 */
uint8_t getEvent() {
    uint8_t event = EVENT_NONE;
    event = key_getEvent();
    return event;
}

uint16_t counter = 0;

/** @brief  Checks buttons and color sensors and print output on 
 *          maroon shield.
 *
 *  @param  enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 *	
 *  @return void	
 */
void handle_event(uint8_t event) {
    
    uint8_t has_maroon = maroon_connected();

    if (event==EVENT_KEY1) {
        
        return;
    } 

    if (event==EVENT_KEY2) {
	    
        return;
    }
	
    if (event==EVENT_KEY3) {
        /** Returns color from surface as 24bit RGB value and writes
         *  in 32 bit variable rgb.
         */
        uint32_t rgb = surface_getColorRGB();
						
        rgb_color_to_string(rgb);
        maroon_print(rgb_str);		
		
        return;
    }
}

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/

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
	    
    maroon_welcome();

}

/************************************************************************/
/* LOOP                                                       */
/************************************************************************/

void loop() {
    /**	Check if monitor voltage is ok. If below nibo_setMonitorVoltage()
     *  program will be interrupted and LED will blink SOS (3 times short,
     *  3 times long, 3 times short). This is important since low voltage
     *  will lead to other sensor outputs.
     */
    nibo_checkMonitorVoltage();

    // Wait until new set of analog values has been measured
    analog_wait_update();

    uint8_t event = getEvent();
    handle_event(event);
}
