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
	int c = key_get_state();
	if (c==KEY_STATE_1 ) return EVENT_KEY1;
	if (c==KEY_STATE_2 ) return EVENT_KEY2;
	if (c==KEY_STATE_3 ) return EVENT_KEY3;
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
            motpwm_stop();
            motpid_stop(1);
            odometry_reset();
            
            run = 0;
            state = 2;
        break;
        
        // Running wheels for alignment
        case 2: 
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }                
                
            if(key_getEvent()==EVENT_KEY1 && run == 0) {
                motpwm_setLeft(300);
                motpwm_setRight(300);
                run = 1;
                state = 3;
            }    
        break;

        // Stopping wheels where odometry counter is 20
        case 3:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }                
                
            int odometry_counter = 20;    
                
            if (odometry_getLeft(0) >= odometry_counter) {
                motpwm_setLeft(0);
                led_set(1, 0);
                led_set(2, 1);
            }
            
            if (odometry_getRight(0) >= odometry_counter) {
                motpwm_setRight(0);
                led_set(4, 0);
                led_set(3, 1);
            }
            
            if(odometry_getLeft(0) >= odometry_counter && odometry_getRight(0) >= odometry_counter) {
                run = 0;  
                state = 4;              
            }
        break;

        // Moving 1.5m straight ahead
        case 4:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }
            
            // 100 odometry ticks = 141 mm --> 1500mm = 1064
            int dist_in_ticks = 200;
            odometry_reset();
            
            if(key_getEvent()==EVENT_KEY2 && run == 0) {              
                motpid_setTargetRel(dist_in_ticks, dist_in_ticks, 40);
                run = 1;
            }
            
            if(odometry_getLeft(0)>=dist_in_ticks && odometry_getRight(0)>=dist_in_ticks) {
                run = 0;
                state = 5;
            }   
        break;

        case 5:

        break;

        case 6:

        break;

        case 7:

        break;

        case 8:
 
        break;
    }
}

void setup() {
	led_init();
	analog_init();
	motpwm_init();
	motpid_init();
	odometry_init();
}

void loop() {
    stateMachine();
}