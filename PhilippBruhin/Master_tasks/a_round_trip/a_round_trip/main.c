/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

enum {
    EVENT_NONE              =  0,
    EVENT_KEY1              =  1,
    EVENT_KEY2              =  2,
    EVENT_KEY3              =  3,
};

int run = 0;
unsigned char state = 1;


/** @brief  Makes an LED blink with 80ms on and 120ms off
 *
 *  @param  led     LED number
 *  @param  count   How many times led should blink
 *
 *  @return void
 */
void blink_led(int led, int count) {
	while (count--) {
		led_set(led, 1);
		delay(80);
		led_set(led, 0);
		delay(120);
	}
}


/** @brief  Returns which button is clicked or EVENT_NONE
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 */
int key_getEvent() {
	int c = key_get_char();
	if (c=='a') return EVENT_KEY1;
	if (c=='b') return EVENT_KEY2;
	if (c=='c') return EVENT_KEY3;
	return EVENT_NONE;
}


/************************************************************************/
/* STATEMACHINE                                                         */
/************************************************************************/
void stateMachine() {
    
    switch( state ) {
        
        // Start position
        case 1:
            led_setall(1,0,0,1);
            state = 2;
        break;
        
        // Bringing wheels into start position
        case 2:    
            if(key_getEvent()==EVENT_KEY1 && run == 0) {
                motpwm_setLeft(100);
                motpwm_setRight(100);
                run = 1;
                odometry_reset();
            }
                    
            if (odometry_getLeft(0)>=10) {
                motpwm_setLeft(0);
                led_set(1, 0);
                led_set(2, 1);
            }
                    
            if (odometry_getRight(0)>=10) {
                motpwm_setRight(0);
                led_set(4, 0);
                led_set(3, 1);
            }
                    
            state = 3;
        break;

        case 3:
            state = 4;
        break;

        case 4:
            state = 5;
        break;

        case 5:
            state = 6;
        break;

        case 6:
            state = 7;
        break;

        case 7:
            state = 8;
        break;

        case 8:
            state = 1;
        break;
    }
}

void setup() {
	led_init();
	analog_init();
	motpwm_init();
	motpid_init();
	odometry_init();
    	
	// PID controller setpoint to zero
	motpid_stop(1);   
}

void loop() {
    stateMachine();
}