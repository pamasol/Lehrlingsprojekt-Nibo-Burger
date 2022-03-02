/**
 *	NIBO Burger � Pamasol electronic project for apprentices
 *	Master task C) Follow me
 *	Setup: IR-Bricks in slots FLL, FL, FR, FRR, maroon shield mounted
 *  Instructions:
 *      1. Place robot on the ground an press key 1, 2 or 3 for starting.
 *      2. Hold hand about 10cm in front of the robot and it will follow.
 *      3. Press key 1, 2 or 3 for stopping the robot.
 *  Worth knowing:
 *      Program works with an event handler. In programming, an event handler
 *      is a callback routine that operates asynchronously once an event
 *      takes place.
 *      This program checks for events with getEvent() and in a second step
 *      it runs handle_event() to act. 
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

uint8_t obstacle_val;   // Numbers from 0 up to 127

/** @brief  Limit number range to 0-255 (8 bit) 
 *
 *  @param  sensor  Sensor number (1,2,3 or 4)
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
uint8_t obstacle_getEvent() {
	
    /** Since static, this variable is like a global variable and
     *  therefore assigned on the first function call only. 
     */    
    static uint8_t state = 0;
    
    uint8_t nstate = 0;
      
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
    /** Building three areas based on the values of 4 sensors.
     *  Area center: sensor left + sensor right
     *  Area left: sensor left outside + sensor left
     *  Area right: sensor right outside + sensor right
     */  
    uint8_t cc = l + r;
    ll += l;
    rr += r;
    
	// Check highest value and write it in obstacle_val
    obstacle_val = (ll>rr)?ll:rr;
    if (cc>obstacle_val) {
        obstacle_val = cc;
    }

    // Analyze obstacle location and set nstate 
    if ((obstacle_val==cc) || (ll==rr)) {
        if (ll>rr) {
            // Hand slightly left
            nstate = EVENT_HAND_L;
        } else if (ll<rr) {
            // Hand slightly right
            nstate = EVENT_HAND_R;
        } else {
            // Hand straight ahead
            nstate = EVENT_HAND_C;
        }
    } else if (ll>rr) {
        // Hand left
        nstate = EVENT_HAND_LL;
    } else {
        // Hand right
        nstate = EVENT_HAND_RR;
    }

    /** Analyze hand distance
     *  more than 50 --> hand to close to the sensor
     *  less than  8 --> no hand detected
     */  
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
 *          If no button is clicked it checks for obstacle events.
 *          If no obstacle event it returns event none.
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
    event = obstacle_getEvent();
    return event;
}

uint8_t run = 0;
uint16_t counter = 0;

/** @brief  Checks buttons and obstacle sensors and controls the
 *          motors based on these events.
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
            obstacle_getEvent();
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

    if (event==EVENT_TO_CLOSE) {
            motpid_stop(1);                             // Stop abrupt 
            if (has_maroon) led_setall(1, 0, 0, 1);
            if (!has_maroon) led_setall(1, 1, 1, 1);

        } else if (event==EVENT_NO_HAND) { 
            motpid_stop(0);                             // Stop smoothly
            led_setall(0, 0, 0, 0);

        } else if (event==EVENT_HAND_L) {
            motpid_setSpeed(+20,+30);                   // Hand slightly left
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 1, 0, 0);

        } else if (event==EVENT_HAND_C) {
            motpid_setSpeed(+30,+30);                   // Hand straight ahead
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 1, 1, 0);

        } else if (event==EVENT_HAND_R) {
            motpid_setSpeed(+30,+20);                   // Hand slightly right
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(0, 0, 1, 0);

        } else if (event==EVENT_HAND_LL) {
            motpid_setSpeed(-10,+20);                   // Hand left
            if (has_maroon) led_setall(0, 0, 0, 0);
            if (!has_maroon) led_setall(1, 1, 0, 0);

        } else if (event==EVENT_HAND_RR) {
            motpid_setSpeed(+20,-10);                   // Hand right
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
    // Blink SOS with LEDs when voltage too low
    nibo_checkMonitorVoltage();
    // Do not run loop if analog value does not change
    analog_wait_update();
    
    // Update display
    maroon_loop();
    
    uint8_t event = getEvent();
    handle_event(event);
}
