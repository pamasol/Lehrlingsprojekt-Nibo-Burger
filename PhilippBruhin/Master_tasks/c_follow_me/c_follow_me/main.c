/**
 *	NIBO Burger – Pamasol electronic project for apprentices
 *	Master task C) Follow me
 *	Setup: IR-Bricks in slots FLL, FL, FR, FRR, maroon shield mounted
 */

#include "niboburger/robomain.h"
#include "maroon.h"


uint8_t progmode = 0;

enum {
    EVENT_NONE              =  0,
    EVENT_KEY1              =  1,
    EVENT_KEY2              =  2,
    EVENT_KEY3              =  3,
  
    EVENT_NO_HAND           = 20,
    EVENT_TO_CLOSE          = 21,
    EVENT_HAND_C            = 22,
    EVENT_HAND_L            = 23,
    EVENT_HAND_R            = 24,  
    EVENT_HAND_LL           = 25,
    EVENT_HAND_RR           = 26
};


uint8_t key_getEvent();


/** @brief  Makes an LED blink with 80ms on and 120ms off
 *
 *  @param  led     LED number
 *  @param  count   How many times led should blink
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

int8_t obstacle_pos;    // Values from -1 up to +2
uint8_t obstacle_val;   // Values from 0 up to 255

/** @brief  Limit number range to 0-255 (8 bit) 
 *
 *  @param  sensor  Sensor number
 *	
 *  @return val     Number between 0 and 255
 */
uint8_t getObstSensorValue(uint8_t sensor) {
    uint16_t val = analog_getValueExt(sensor, 2);   // returns 0 - 1024
    if (val & 0xff00) return 0xff;                  // Set max to 255
    return val;
}


/** @brief  Returns an event based on position of the hand.
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_HAND_L, EVENT_HAND_R, EVENT_HAND_C
 *                  EVENT_HAND_LL, EVENT_HAND_RR, EVENT_TO_CLOSE,
 *                  EVENT_NO_HAND
 */
uint8_t follow_getEvent() {
	
	// Since static, this variable is like a global variable and
	// therefore assigned on the first function call only.    
    static uint8_t state = 0;
    
    uint8_t nstate = 0;
      
	// Shift one bit to the right (division by 2)
	// Advantage: number is getting smaller
	// Disadvantage: leads to loss of resolution  
    uint8_t l = getObstSensorValue(ANALOG_FL)>>1;
    uint8_t r = getObstSensorValue(ANALOG_FR)>>1;
    uint8_t ll = getObstSensorValue(ANALOG_FLL)>>1;
    uint8_t rr = getObstSensorValue(ANALOG_FRR)>>1;
    uint8_t cc = l + r;
    ll += l;
    rr += r;
    
	// Three areas: center, left, right
    obstacle_val = (ll>rr)?ll:rr;
    if (cc>obstacle_val) {
        obstacle_val = cc;
    }

    // Analyze obstacle location  
    if ((obstacle_val==cc) || (ll==rr)) {
        if (ll>rr) {
            // Hand slightly left
            obstacle_pos = -1;
            nstate = EVENT_HAND_L;
        } else if (ll<rr) {
            // Hand slightly right
            obstacle_pos = +1;
            nstate = EVENT_HAND_R;
        } else {
            // Hand straight ahead
            obstacle_pos = 0;
            nstate = EVENT_HAND_C;
        }
    } else if (ll>rr) {
        // Hand left
        obstacle_pos = -2;
        nstate = EVENT_HAND_LL;
    } else {
        obstacle_pos = +2;
        // Hand right
        nstate = EVENT_HAND_RR;
    }

	// Analyze hand distance
	// more than 50 --> hand to close to the sensor
    // less than  8 --> no hand detected
    if (obstacle_val>50) {
            nstate = EVENT_TO_CLOSE;
        } else if (obstacle_val<8) {
            nstate = EVENT_NO_HAND;
    }
 
 	// Only return event if event changed
    if (state!=nstate) {
        state = nstate;
        return state;
    }
    
    return EVENT_NONE;
}

/************************************************************************/
/* KEY                                                                  */
/************************************************************************/

/** @brief  Returns which button has been clicked or EVENT_NONE
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


/** @brief  Returns which button is clicked or EVENT_NONE. If no button
 *          is clicked, it checks if there is an obstacle.
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3,
 *                  EVENT_HAND_L, EVENT_HAND_R, EVENT_HAND_C
 *                  EVENT_HAND_LL, EVENT_HAND_RR, EVENT_TO_CLOSE,
 *                  EVENT_NO_HAND
 */
uint8_t getEvent() {
    uint8_t event = EVENT_NONE;
    event = key_getEvent();
    if (event) return event;
    event = follow_getEvent();
    return event;
}


uint8_t run = 0;
uint16_t counter = 0;


/** @brief  Checks buttons and obstacle sensors and controls the
 *          motors based on these events (state machine).
 *
 *  @param  enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3,
 *                  EVENT_HAND_L, EVENT_HAND_R, EVENT_HAND_C
 *                  EVENT_HAND_LL, EVENT_HAND_RR, EVENT_TO_CLOSE,
 *                  EVENT_NO_HAND
 *	
 *  @return void	
 */
void handle_event(uint8_t event) {
    
    uint8_t has_maroon = maroon_connected();

    if (run==0) {
        if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
            run = 1;
            follow_getEvent();
        }
        return;
    }

    if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
        run = 0;
        motpid_stop(1);
        led_setall(1, 0, 0, 1);
        return;
    }

    if (event==EVENT_TO_CLOSE) {
            motpid_stop(1);                             // stop abrupt 
            if (has_maroon) led_setall(1, 0, 0, 1);
            if (!has_maroon) led_setall(1, 1, 1, 1);

        } else if (event==EVENT_NO_HAND) { 
            motpid_stop(0);                             // stop smoothly
            led_setall(0, 0, 0, 0);

        } else if (event==EVENT_HAND_L) {
            motpid_setSpeed(+20,+30);
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 1, 0, 0);

        } else if (event==EVENT_HAND_C) {
            motpid_setSpeed(+30,+30);
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 1, 1, 0);

        } else if (event==EVENT_HAND_R) {
            motpid_setSpeed(+30,+20);
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 0, 1, 0);

        } else if (event==EVENT_HAND_LL) {
            motpid_setSpeed(-10,+20);
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(1, 1, 0, 0);

        } else if (event==EVENT_HAND_RR) {
            motpid_setSpeed(+20,-10);
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 0, 1, 1);
    
    }  
}


void setup() {
    
    // LED bits as output
    activate_output_group(IO_LEDS);
  
    maroon_setup();
    motpwm_init();
    motpid_init();
    odometry_init();
    analog_init();

	// When switching on robot wait until a button is clicked  
    while (key_getEvent()==EVENT_NONE) {
        // wait...
    }
  
    delay(200);
  
    maroon_welcome();
  
    // PID controller setpoint to zero	  
    motpid_stop(1);
    blink_led(1, 4);
    run = 1;
}


void loop() {
    nibo_checkMonitorVoltage();
    analog_wait_update();
    maroon_loop();
    uint8_t event = getEvent();
    handle_event(event);
}
