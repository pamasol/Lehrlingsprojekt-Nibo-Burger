/**
 *  NIBO Burger � Pamasol electronic project for apprentices
 *  Master task B) Fraidy cat
 *  Setup: IR-Bricks in slots FLL, FL, FR, FRR, no maroon shield mounted
 *  Instructions:
 *      1. Place robot on the ground an press key 1, 2 or 3 for starting.
 *      2. Obstacles can be simulated with hands. The robot will dodge.
 *      3. Press key 1, 2 or 3 for stopping the robot.
 *  Worth knowing:
 *      Program works with an event handler. In programming, an event handler
 *      is a callback routine that operates asynchronously once an event
 *      takes place.
 *      This program checks for events with getEvent() and in a second step
 *      it runs handle_event() to act.
 */

#include <niboburger/robomain.h>

enum {
	EVENT_NONE              =  0,
	EVENT_KEY1              =  1,
	EVENT_KEY2              =  2,
	EVENT_KEY3              =  3,
  
	EVENT_OBSTACLE_DETECTED = 16,
	EVENT_OBSTACLE_CLEAR    = 17,
	EVENT_OBSTACLE_CLEAR_L  = 18,
	EVENT_OBSTACLE_CLEAR_R  = 19
};

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

/************************************************************************/
/* OBSTRACLE                                                            */
/************************************************************************/

int8_t obstacle_pos;    // Numbers from -2 up to +2
uint8_t obstacle_val;   // Numbers from 0 up to 127

/** @brief  Limit number range to 0-255 (8 bit) 
 *
 *  @param  sensor  Sensor number (1,2,3 or 4)
 *	
 *  @return val     Number between 0 and 255
 */
uint8_t getObstSensorValue(uint8_t sensor) {
    /** Returns value between 0 and 1024. Mode is as follows:
     *  ANALOG_PASSIVE = 0: passive measurement (LED off)
     *  ANALOG_ACTIVE  = 1: active measurement (LED on)
     *  ANALOG_DIFFERENTIAL = 2: active - passive measurement (difference)
     */
	uint16_t val = analog_getValueExt(sensor, 2);
	// If value bigger than 255, return 255
	if (val>0xff) return 0xff;
	// else return val
	return val;
}

/** @brief  Returns an event based on an obstacle
 *
 *  @param  reset   1 or any other value for reset except 0
 *	
 *  @return enum    EVENT_NONE, EVENT_OBSTACLE_DETECTED, EVENT_OBSTACLE_CLEAR,
 *                  EVENT_OBSTACLE_CLEAR_R, EVENT_OBSTACLE_CLEAR_L
 */
uint8_t obstacle_getEvent(uint8_t reset) {
	
	/** Since static, this variable is like a global variable and 
	 *  therefore assigned on the first function call only.
     */
	static uint8_t event_last = EVENT_NONE;
  
	if (reset) {
		event_last = EVENT_NONE;
		return EVENT_NONE;
	}
  
	uint8_t event = EVENT_NONE;
	
	/** Shift one bit to the right (division by 2)
     *  11111111 = 255
     *  01111111 = 127
	 *  Advantage: number is getting smaller
	 *  Disadvantage: leads to loss of resolution
     */
	uint8_t l = getObstSensorValue(ANALOG_FL)>>1;
	uint8_t r = getObstSensorValue(ANALOG_FR)>>1;
	uint8_t ll = getObstSensorValue(ANALOG_FLL)>>1;
	uint8_t rr = getObstSensorValue(ANALOG_FRR)>>1;
		
	// Three areas: center, left, right
	uint8_t cc = l + r;
	ll += l;
	rr += r;
	obstacle_val = max3(ll, cc, rr);
  
	// Analyze obstacle location
	if ((obstacle_val==cc) || (ll==rr)) {
		if (ll>rr) {
			// Obstacle slightly left
			obstacle_pos = -1;
		} else if (ll<rr) {
			// Obstacle slightly right
			obstacle_pos = +1;
		} else {
			// Obstacle straight ahead
			obstacle_pos = 0;
		}
	} else if (ll>rr) {
		// Obstacle left
		obstacle_pos = -2;
	} else {
		// Obstacle right
		obstacle_pos = +2;
	}

	/** Analyze obstacle distance with a hysteresis
	 *  from 20 up to 25.
     */ 
	if (obstacle_val>25) {
		event = EVENT_OBSTACLE_DETECTED;
	}
	
	if (obstacle_val<20) {
		event = EVENT_OBSTACLE_CLEAR;
		if (obstacle_val>5) {
			if (obstacle_pos<0) {
				event = EVENT_OBSTACLE_CLEAR_R;
			} else if (obstacle_pos>0) {
				event = EVENT_OBSTACLE_CLEAR_L;
			}
		}
	} 
    
	// Only return event if event changed
	if (event!=event_last) {
		event_last = event;
		return event;
	}
  
	return EVENT_NONE;
}

/************************************************************************/
/* KEY                                                                  */
/************************************************************************/

/** @brief  Returns which button is clicked or EVENT_NONE
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 */
uint8_t key_getEvent() {
	uint8_t c = key_get_char();
	if (c=='a') return EVENT_KEY1;
	if (c=='b') return EVENT_KEY2;
	if (c=='c') return EVENT_KEY3;
	return EVENT_NONE;
}

/** @brief  Checks if a button is clicked and if so, returns it as event.
 *          If no button is clicked it checks for obstacle events.
 *          If no obstacle event it returns event none.
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3,
 *                  EVENT_OBSTACLE_DETECTED, EVENT_OBSTACLE_CLEAR,
 *                  EVENT_OBSTACLE_CLEAR_R, EVENT_OBSTACLE_CLEAR_L
 */
uint8_t getEvent() {
	uint8_t event = EVENT_NONE;
	event = key_getEvent();
	if (event) return event;
	event = obstacle_getEvent(0);
	return event;
}

uint8_t run = 0;

void setup() {
	
	// LED bits as output
	activate_output_group(IO_LEDS);

	motpwm_init();
	motpid_init();
	odometry_init();
	analog_init();

	// When switching on robot wait until a button is clicked
	while (key_getEvent()==EVENT_NONE) {
		// wait...
	}

	delay(200);

	// PID controller setpoint to zero	
	motpid_stop(1);

	blink_led(2, 4);

	run = 1;
}

/** @brief  Checks buttons and obstacle sensors and controls the
 *          motors based on these events.
 *
 *  @param  enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3,
 *                  EVENT_OBSTACLE_DETECTED, EVENT_OBSTACLE_CLEAR,
 *                  EVENT_OBSTACLE_CLEAR_R, EVENT_OBSTACLE_CLEAR_L
 *	
 *  @return void	
 */
void handle_event(uint8_t event) {
	
    /** If robot is not running and button has clicked
     *  Please note initial setup. When main switch has switched on, it is
     *  waiting in the setup function (while loop) until a key has pressed.
     *  Then run variable is set to 1 (still in setup function) and robot
     *  starts moving. Thus the program is not running trough the loop 
     *  over and over although not a single button has been pressed yet.
     */
	if (run==0) {
		if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
			run = 1;
			obstacle_getEvent(1);
		}
		return;
	} 
  
	// If NIBO is running and button has clicked
	if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
		run = 0;
		motpid_stop(1);
		led_setall(1, 0, 0, 1);
		return;
	}

	if (event==EVENT_OBSTACLE_DETECTED) {
		/** obstacle_pos negative -> obstracle on left side
		 *  obstacle_pos positive -> obstracle on right side
         */
		led_setall(obstacle_pos<=0, 0, 0, obstacle_pos>=0);
		if (obstacle_pos>0) {
			motpid_setSpeed(-25,+15);
		} else if (obstacle_pos<0) {
			motpid_setSpeed(+15,-25);
		} else {
			motpid_setSpeed(-20,-20);
		}
	}
      
	if (event==EVENT_OBSTACLE_CLEAR) {
		led_setall(0, 0, 0, 0);
		motpid_setSpeed(+30,+30);
	}

	if (event==EVENT_OBSTACLE_CLEAR_L) {
		led_setall(0, 0, 1, 0);
		motpid_setSpeed(+20,+30);
	}
	
	if (event==EVENT_OBSTACLE_CLEAR_R) {
		led_setall(0, 1, 0, 0);
		motpid_setSpeed(+30,+20);
	}
}

void loop() {
    // Do not run loop if analog value does not change
    analog_wait_update();
    uint8_t event = getEvent();
    handle_event(event);
}
