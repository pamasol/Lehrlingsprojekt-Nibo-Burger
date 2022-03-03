/**
 *  NIBO Burger – Pamasol electronic project for apprentices
 *  Master task E) Rabbit warren
 *  Setup: IR-Bricks in slots FLL, FL, FR, FRR, maroon shield mounted
 *  Instructions:
 *      1. Switch on robot and click key 3 for calibration
 *	        a. Put robot on black area and click key 1. Wait for LEDs blinking 5 times.
 *	        b. Put robot on white area and click key 2. Wait for LEDs blinking 5 times.
 *	        c. Click key 3 for storing the calibration values and wait for blinking LEDs.
 *      2. Put robot to start position and click...
 *  Worth knowing:
 *  
 *  
 */

#include <niboburger/robomain.h>
#include <stdlib.h>
#include <string.h>

// Line tracking
#define FLOOR_VALUE 40              // Light-gray floor: 40, Medium-gray floor: 30
int8_t stear;                       // Value between -64 (line right) and +64 (line left)
volatile int8_t state;              // Value between -2 and +2
uint16_t loose_cnt = 550;           // Max 550 and decreases when state +2 or -2
uint8_t obst_max;                   // Obstacles detected by IR-Bricks. Mas value is 127.

// Color detection (start and finish) 
uint8_t color;                      // 1 for blue, 2 for red, 0 for neither
uint8_t colstabcnt = 0;             // Color stable count. Max value is 254.

// Display control
uint8_t display_mode;
                     
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
    led_blink(1,1,1,1);
	
    // Waiting in calibration mode for push of a key
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

/** @brief  Checks if surface color is red (right sensor) or blue
 *  (left sensor) and integrates the time while the same color is
 *  pending.
 *
 *  @param  red     Int value of red color sensor (value between 0 and 127)
 *  @param  green   Int value of green color sensor (value between 0 and 127)
 *  @param  blue    Int value of blue color sensor (value between 0 and 127)
 *
 *  @return void
 */
static void check_color(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t col=0;
    if (red<10) {
        // maybe blue
        if ((green>30)&&(green<70)&&(blue>50)) {
            // is blue
            col=1;
        }
    } else if (red>50) {
        // maybe red
        if ((green<20)&&(blue<20)) {
            // is red
            col=2;
        }    
    }
    // Same color as on last function call
    if (color==col) {
        if (col) {
            if (colstabcnt<255) {
                colstabcnt++;
            }
        } else {
            colstabcnt /= 2;
        }
    // Other color as on last function call    
    } else {
        if (col==0) {
            colstabcnt /= 2;
        }
        color = col;
    }
}

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
  
    /** If difference between bcl and bcr is smaller then 12, the robot could 
     *  be at the finish or start point (red or blue surface).
     */      
    if ((dir < -12)||(dir > +12)) {
        color = 0;
    } else {
        check_color(br, bc, bl);
    }

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

    switch (color) {
        case 0: led_setall(0,0,0,0); break;     // Neither red (right) nor blue (left) sensor
        case 1: led_setall(0,1,1,0); break;     // On blue surface
        case 2: led_setall(1,0,0,1); break;     // On red surface
    }
}

/************************************************************************/
/* SETUP - called once at startup                                       */
/************************************************************************/

void setup() {
    led_init();
    analog_init();
    motpwm_init();
  
    /** ANALOG_BCL and ANALOG_BCR are not activated by default as the
     *  other surface sensors when surface_init() is called. They need
     *  to be activated separately.
     */
    surface_init();
    analog_setExtToggleMode(ANALOG_BCL, 1);
    analog_setExtToggleMode(ANALOG_BCR, 1);

    led_blink(0,1,1,0);

    // After start up waiting for push of a key
    while (1) {
        char c = key_get_char();
        if (c=='c') {
            calibrate();
            display_mode=2;
            break;
        } else if (c=='a') {
            display_mode=1;
            break;
        } else if (c=='b') {
            display_mode=2;
            break;
        }
    }
	
}

/************************************************************************/
/* LOOP -  loops consecutively                                          */
/************************************************************************/

void loop() {
    nibo_checkMonitorVoltage();
    analog_wait_update();

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
  
    /** Check if robot on blue or red surface and if so, reduce speed. If
     *  on one color for some time, stop immediately.
     */ 
    if (color) {
        left /= 2;
        right /= 2;
    }
    if (colstabcnt>=8) {
        left=right=0;
    }
   
    // check for obstacles and stop if there are any
    uint8_t l  = min(127, analog_getValueExt(ANALOG_FL, 2) /8);
    uint8_t r  = min(127, analog_getValueExt(ANALOG_FR, 2) /8);
    uint8_t ll = min(127, analog_getValueExt(ANALOG_FLL, 2)/8);
    uint8_t rr = min(127, analog_getValueExt(ANALOG_FRR, 2)/8);
  
    obst_max = max4(l,r,ll,rr);
    if (obst_max>8) {
        left = right = 0;
    }
  
    // Set motor speed based on state and on blue or red surface color
    motpwm_setLeft(left);
    motpwm_setRight(right);
	
}
