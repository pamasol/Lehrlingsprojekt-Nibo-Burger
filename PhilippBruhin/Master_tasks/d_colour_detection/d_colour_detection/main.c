/**
 *  NIBO Burger – Pamasol electronic project for apprentices
 *  Master task D) Colour detection
 *  Setup: maroon shield mounted
 *  Instructions:
 *      If sensors should be calibrated, start with step 1. Otherwise jump to step 2.    
 *      1. Switch on robot and put it on black surface.
 *          a. Click key 1. Wait until LED 2 (blue LED left) did blink 5 times.
 *          b. Put robot on white area and click key 2. Wait until LED 3 (blue LED right)
 *             did blink 5 times.
 *          c. Click key 3 for storing the calibration values and wait until LED 1 and 4
 *             (red LEDs) did blink 5 times.
 *      2. Place robot on colored surface and click key 2 for getting colors red, green
 *         blue, yellow, black and white. 
 *      3. Click key 3 for getting the color as hex value.
 *  Worth knowing:
 *      This program makes heavily use of RGB color pattern. R represents the red value,
 *      G the green and B the blue. Examples:
 *      Red     #ff0000 --> 1111 1111 0000 0000 0000 0000
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
uint8_t color = COL_NONE;

/************************************************************************/
/* HELPER FUNCTIONS                                                     */
/************************************************************************/

/** @brief  Makes an LEDs blink with 100ms on and 100ms off. Cli clears
 *  the global interrupt flag in SREG so prevent any form of interrupt
 *  occurring. While sei sets the bit and switches interrupts on.
 *
 *  @param  l1      LED 1 must be 0 for being off or 1 for blinking
 *  @param  l2      LED 2 must be 0 for being off or 1 for blinking
 *  @param  l3      LED 3 must be 0 for being off or 1 for blinking
 *  @param  l4      LED 4 must be 0 for being off or 1 for blinking
 *
 *  @return void
 */
void led_blink(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4) {
	cli();
	for (uint8_t i=0; i<5; i++) {
		led_setall(l1,l2,l3,l4);
		delay(100);
		led_setall(0,0,0,0);
		delay(100);
	}
	sei();
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

/** @brief  Calibrates RGB sensors based on black and white surface. 
 *  Calibration values are stored in the EEPROM and will be persistent
 *  when reprogramming. So this step does not to be repeated with every
 *  try.
 *
 *  @param  -
 *
 *  @return void
 */
void calibrate() {

	// Stay in calibration mode for all calibration steps
	while (1) {
		char c = key_get_char();
		if (c=='a') {
			delay(200);
			surface_calibrateBlack();
			led_blink(0,1,0,0);
		} else if  (c=='b') {
			delay(200);
			surface_calibrateWhite();
			led_blink(0,0,1,0);
		} else if  (c=='c') {
			surface_writePersistent();
			led_blink(1,0,0,1);
			return;
		}
	}
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

    /**	CLICKING KEY 1
     *  Starts sensor calibration for black surface, followed by white.
     */
    if (event==EVENT_KEY1) {
        calibrate();
        return;
    } 

    /**	CLICKING KEY 2
     *  Returns color as name. Works for red, blue, green, yellow,
     *  white and black.
     */
    if (event==EVENT_KEY2) {
	    
        // Values from 0 up to 1023 when calibrated correct
        int16_t r = surface_get(SURFACE_R);
        int16_t g = surface_get(SURFACE_C);
        int16_t b = surface_get(SURFACE_L);
        // The pure yellow is formed by overlapping Red=1, Green=1 and Blue=0.
        int16_t y = (r+g)/2;
        
        int16_t rg = (r-g);
        int16_t by = (b-y);
        int16_t i = r+g+b;
        
        // Debugging purpose. Puts value of color on display.
        // char color_value[20];
        // sprintf(color_value, "%d", by);
        // maroon_print(color_value);

        // Calculate color based in red, green and blue intensity
        if ((abs(rg)<100) && (abs(by)<100)) {
            if (i<200) color = COL_BLACK;
            if (i>2500) color = COL_WHITE;
        }
        if (rg>500) {
            if (g<200) color = COL_RED;
            else color = COL_YELLOW;
        }
        if ((rg<100)&&(by>200)) {
            color = COL_BLUE;
        }
        if ((rg<100)&&(by<-300)) {
            color = COL_GREEN;
        }

        // Show color on display
        if (color==COL_BLACK) {
            maroon_print("black\n");	
        } else if (color==COL_WHITE) {
            maroon_print("white\n");	
        } else if (color==COL_RED) {
            maroon_print(" red\n");	
        } else if (color==COL_BLUE) {
            maroon_print(" blue\n");	
        } else if (color==COL_GREEN) {
            maroon_print(" green\n");	
        } else if (color==COL_YELLOW) {
            maroon_print(" yellow\n");	
        } else {
            maroon_print(" unknown\n");
        }            
                     
        return;
    }
	
    
    /**	CLICKING KEY 3
     *  Returns color as hex value. For example red as #ff0000 or
     *  yellow as #ffff00.
     */
    if (event==EVENT_KEY3) {

        // Writing 24 bit RGB value in 32 bit variable
        uint32_t rgb = surface_getColorRGB();
		
        // transform 32 bit int to string with hex value				
        rgb_color_to_string(rgb);
        
        // Show color on display
        maroon_print(rgb_str);		
		
        return;
    }
}

/************************************************************************/
/* SETUP - called once at startup                                       */
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
/* LOOP -  loops consecutively                                          */
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

    // Look for event and handle event
    uint8_t event = getEvent();
    handle_event(event);
}
