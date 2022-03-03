/**
 *  NIBO Burger – Pamasol electronic project for apprentices
 *  Master task E) Rabbit warren
 *  Setup: IR-Bricks in slots FLL, FL, FR, FRR, maroon shield mounted
 *  Instructions:
 *      If sensors should be calibrated, start with step 1. Otherwise jump to step 2.
 *      1. Switch on robot and put it on black surface.
 *          a. Click key 1. Wait until LED 2 (blue LED left) did blink 5 times.
 *          b. Put robot on white area and click key 2. Wait until LED 3 (blue LED right)
 *             did blink 5 times.
 *          c. Click key 3 for storing the calibration values and wait until LED 1 and 4
 *             (red LEDs) did blink 5 times.
 *      2. ...
 *      3. ...
 *  Worth knowing:
 *  
 *  
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
    
    EVENT_OBSTRACLE         = 20,
};

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

/** @brief  ??
 *  
 *  
 *  
 *
 *  @param  -
 *
 *  @return void
 */
uint8_t obstacle_getEvent() {
    uint8_t obst_max;                   // Obstacles detected by IR-Bricks. Mas value is 127.
    	
    // check for obstacles and stop if there are any
    uint8_t l  = min(127, analog_getValueExt(ANALOG_FL, 2) /8);
    uint8_t r  = min(127, analog_getValueExt(ANALOG_FR, 2) /8);
    uint8_t ll = min(127, analog_getValueExt(ANALOG_FLL, 2)/8);
    uint8_t rr = min(127, analog_getValueExt(ANALOG_FRR, 2)/8);
    
    obst_max = max4(l,r,ll,rr);
    if (obst_max>8) {
        return EVENT_OBSTRACLE;
    } else {
       return EVENT_NONE; 
    }

}

/************************************************************************/
/* LINE DETECTION AND FOLLOWING                                         */
/************************************************************************/

#define FLOOR_VALUE 40              // Light-gray floor: 40, Medium-gray floor: 30
int8_t stear;                       // Value between -64 (line right) and +64 (line left)
volatile int8_t state;              // Value between -2 and +2
uint16_t loose_cnt = 550;           // Max 550 and decreases when state +2 or -2

/** @brief  Sets the state variable from -2 up to +2 based on the 
 *  values from the three surface sensors.
 *
 *  Caution: Function is called from the IRQ context!
 *  
 *  Hooks are a category of function that allows base code to call
 *  extension code. In this case the function is called every time
 *  when all analog channels did measure a new value. No need to
 *  call this function somewhere in this main.c file. 
 *
 *  @param  -
 *
 *  @return void
 */
void analog_irq_hook() {
    /** surface_get() returns values between 0 and 1023. The min() function
     *  returns the lesser value. So max value is 127.
     *
     *  ANALOG_BCL: Floor sensor left  -->  light from middle sensor
     *  ANALOG_BL:  Floor sensor left
     *  ANALOG_BC:  Floor sensor center
     *  ANALOG_BR:  Floor sensor right
     *  ANALOG_BCR: Floor sensor right -->  light from middle sensor
     */
    uint8_t bcl = min(127, surface_get(SURFACE_CL)/8);
    uint8_t bl  = min(127, surface_get(SURFACE_L) /8);
    uint8_t bc  = min(127, surface_get(SURFACE_C) /8);
    uint8_t br  = min(127, surface_get(SURFACE_R) /8);
    uint8_t bcr = min(127, surface_get(SURFACE_CR)/8);
    
    /** Line left: +64 >= stear > 0
     *  Line right: -64 <= stear < 0
     */
    int8_t dir = (int8_t)bcr-(int8_t)bcl;
    stear = constrain(dir, -64, +64);

    if (bc < FLOOR_VALUE/4) {
        state = 0;                              // Line is in the center
    } else if (bc < FLOOR_VALUE) {
        // Line detected with center sensor
        if (bcl<bcr)  state = -1;               // Line slightly left
        if (bcl>bcr)  state = +1;               // Line slightly right
        if (bcl==bcr) state =  0;               // Line is in the center
    } else {
        // Line lost with center sensor
        if (state==-1) state = -2;              // Line was previously on the left
        if (state==0)  state = +2;              // Line was previously in the center (guessing)
        if (state==+1) state = +2;              // Line was previously on the right
    }
}

/** @brief  
 *  
 *
 *  @param  -
 *
 *  @return void
 */
int8_t follow_line() {
    int16_t left=0, right=0;

    /** Set motor speed based on current line state. State comes from
        *  analog_irq_hook()
        */
    switch (state) {
        // Line was previously on the left
        case -2:
            left=-600; right= 900; break;
        // Line slightly left
        case -1:
            left= 600; right= 1000; loose_cnt = 550; break;                 // loose_cnt reset
        // Line is in the center
        case  0:
            left= 900-stear; right= 900+stear; loose_cnt = 550; break;      // loose_cnt reset
        // Line slightly right
        case +1:
            left= 1000; right= 600; loose_cnt = 550; break;                 // loose_cnt reset
        // Line was previously on the right or was previously (guessing)
        case +2:
            left= 900; right=-600; break;
    }
    
    /** If loose_cnt is not reset on a regularly basis, the robot probably
        *  lost the line. In this case the robot waits a moment and then rotates
        *  around its own axis, trying to find the line again.
        */  
    if (loose_cnt>500) {
        // Waiting...
        loose_cnt--;
    } else if (loose_cnt) {
        // Spin on the spot...
        loose_cnt--;
        if (state>0) {left=+800; right=-800;}
        if (state<0) {left=-800; right=+800;}
    } else {
        // Line finally lost
        left = right = 0;
    }
  
    // Set motor speed based on state and on blue or red surface color
    motpwm_setLeft(left);
    motpwm_setRight(right);
}

/************************************************************************/
/* EVENT HANDLING                                                       */
/************************************************************************/

uint8_t run = 0;
uint8_t obstacle_flag = 0;

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
 *
 *  @param  -
 *	
 *  @return enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 */
uint8_t getEvent() {
    uint8_t event = EVENT_NONE;
    event = key_getEvent();
    if (event) return event;
    event = obstacle_getEvent();
    return event;
}

/** @brief  Checks buttons and runs robot based on key.
 *
 *  @param  enum    EVENT_NONE, EVENT_KEY1, EVENT_KEY2, EVENT_KEY3
 *	
 *  @return void	
 */
void handle_event(uint8_t event) {

    /**	CLICKING KEY 1
     *  Starts sensor calibration for black surface, followed by white.
     *  Works only if robot is not running
     */
    if (event==EVENT_KEY1 && run==0) {
        calibrate();
        return;
    }    

    /**	CLICKING KEY 2
     *  Robot starts following the line.
     */
    if (event==EVENT_KEY2 || obstacle_flag) {
        run=1;       
        return;
    }	
    
    /**	CLICKING KEY 3
     *  Stop robot following the line
     */
    if (event==EVENT_KEY3) {
        run=0;
        return;
    }
    
    if (event==!EVENT_OBSTRACLE && run) {
        follow_line();   
    } else {
        motpwm_setLeft(0);
        motpwm_setRight(0);
    }
}

/************************************************************************/
/* SETUP - called once at startup                                       */
/************************************************************************/

void setup() {
    led_init();
    analog_init();
    motpwm_init();
    maroon_setup();
  
    /** ANALOG_BCL and ANALOG_BCR are not activated by default as the
     *  other surface sensors when surface_init() is called. They need
     *  to be activated separately.
     */
    surface_init();
    analog_setExtToggleMode(ANALOG_BCL, 1);
    analog_setExtToggleMode(ANALOG_BCR, 1);
    
    nibo_setMonitorVoltage(3800);
    delay(200);
    
    maroon_welcome();
	
}

/************************************************************************/
/* LOOP -  loops consecutively                                          */
/************************************************************************/

void loop() {
    nibo_checkMonitorVoltage();
    analog_wait_update();
    uint8_t event = getEvent();
    handle_event(event);	
}
