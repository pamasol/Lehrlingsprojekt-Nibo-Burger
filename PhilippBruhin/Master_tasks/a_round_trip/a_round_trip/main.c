/**
 *  NIBO Burger – Pamasol electronic project for apprentices
 *  Master task A) Round trip
 *  Setup: IR-Bricks and maroon shield not mounted
 *  Instructions:
 *  1. Lift robot off the ground an press key 1 for left wheel alignment.
 *  2. Press key 1 again for right wheel alignment.
 *  3. Place robot on the ground at its start position an press key 2.
 *  4. Reset always possible by clicking key 3.
 */

#include "./niboburger/robomain.h"

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
 *  @param  led     LED number (1, 2, 3 or 4)
 *  @param  count   How many times led should blink (1 up to 32767)
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
    
    static const int odometry_init_counter = 20;
    
    /** @brief  Odometry ticks calculated and trial-and-error for 1500mm
     *          straight ahead on different floors.
     *
     *  @calculation
     *          100 odometry ticks = 141 mm
     *          1500mm = 1064 odometry ticks
     *
     *  @trialled
     *          Pamasol (rough) floor = 1064 
     *          Wood plate = 1064
     */
    static const int dist_in_ticks = 1064;
    
    /** @brief  Odometry ticks calculated and trial-and-error for 1500mm
     *          straight ahead on different floors.
     *
     *  @calculation
     *          114mm diameter (space between wheels) --> 358mm perimeter
     *          179mm half perimeter --> 179mm = 127 ticks
     *
     *  @trialled
     *          Pamasol (rough) floor =  133
     *          Wood plate = 130
     */
    static const int turn_in_ticks = 133;
       
    switch( state ) {
        
        // State 1: Start and reset position
        case 1:
            led_setall(1,0,0,1);
            motpwm_stop();
            motpid_stop(1);
            odometry_reset();
            
            run = 0;
            state = 2;
        break;
        
        // State 2: Running left wheel for alignment
        case 2: 
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }                
                
            if(key_getEvent()==EVENT_KEY1 && run == 0) {
                motpwm_setLeft(300);
                run = 1;
            }    
            
            if (odometry_getLeft(0) >= odometry_init_counter) {
                motpwm_setLeft(0);
                run = 0;
                led_set(1, 0);
                blink_led(2, 1);

                state = 3;
            }                  
        break;

        // State 3: Running right wheel for alignment
        case 3:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }
            
            if(key_getEvent()==EVENT_KEY1 && run == 0) {
                motpwm_setRight(300);
                run = 1;
            }
            
            if (odometry_getRight(0) >= odometry_init_counter) {
                motpwm_setRight(0);
                run = 0;
                led_set(4, 0);
                blink_led(3, 1);

                state = 4;
            }
        break;

        // State 4: Moving 1.5m straight ahead
        case 4:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }
                       
            if(key_getEvent()==EVENT_KEY2 && run == 0) {
                odometry_reset();
                // Delay when clicking button 2 before moving
                blink_led(2, 4);              
                motpid_setTargetRel(dist_in_ticks, dist_in_ticks, 40);
                run = 1;
            }
            
            if(odometry_getLeft(0)>=dist_in_ticks && odometry_getRight(0)>=dist_in_ticks) {
                run = 0;
                state = 5;
            }   
        break;

        // State 5: Turn 180 degree
        case 5:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }

            if(run == 0) {
                odometry_reset();
                motpid_setTargetRel(turn_in_ticks, -turn_in_ticks, 40);
                run = 1;
            }
            
            if(odometry_getLeft(0)>=turn_in_ticks && odometry_getRight(0)<=turn_in_ticks) {
                run = 0;
                state = 6;
            }        
        break;

        // State 6: Moving 1.5m straight ahead
        case 6:
            if (key_getEvent()==EVENT_KEY3) {
                state = 1;
            }
                                                
            if(run == 0) {
                odometry_reset();
                motpid_setTargetRel(dist_in_ticks, dist_in_ticks, 40);
                run = 1;
            }
            
            if(odometry_getLeft(0)>=dist_in_ticks && odometry_getRight(0)>=dist_in_ticks) {
                run = 0;
                state = 1;
            }
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
