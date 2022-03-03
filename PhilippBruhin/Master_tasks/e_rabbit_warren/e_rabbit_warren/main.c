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

int8_t stear;                       // Value between -64 and +64

uint8_t display_mode;
volatile int8_t state;              // Value between -2 and +2
uint8_t color;                      // ???
                     
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
 .
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


/* diese Funktion wird automatisch aufgerufen. Der Aufruf erfolgt 
   immer nachdem alle analogen Kanäle einen neuen Wert gemessen
   haben. Da die Funktion so häufig aufgerufen wird, sollte sie 
   möglichst kurz sein und keine Wartezeiten (delay(), etc...)
   oder ausgiebige Berechnungen enthalten!
*/

static void check_color(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t col=0;
    if (r<10) {
        // maybe blue
        if ((g>30)&&(g<70)&&(b>50)) {
            // is blue
            col=1;
        }
    } else if (r>50) {
        // maybe red
        if ((g<20)&&(b<20)) {
            // is red
            col=2;
        }    
    }
    if (color==col) {
        if (col) {
            if (colstabcnt<255) {
                colstabcnt++;
            }
        } else {
            colstabcnt /= 2;
        }
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
 *  Hooks are a category of function that allows base code to
 *  call extension code. In this case the function is called after a
 *  complete pass through all sampling channels. No need to call this
 *  function somewhere in this main.c file. 
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
  
    /** ???
     *  
     */      
    if ((dir < -12)||(dir > +12)) {
        color = 0;
    } else {
        check_color(br, bc, bl);
    }



    if (bc<FLOOR_VALUE/4) {
    state = 0; // Linie liegt sehr mittig!
    } else if (bc<FLOOR_VALUE) {
    // Linie detektiert
    if (bcl<bcr)  state = -1; // Linie liegt links
    if (bcl>bcr)  state = +1; // Linie liegt rechts
    if (bcl==bcr) state =  0; // Linie liegt mittig
    } else {
    // Linie verloren
    if (state==-1) state = -2; // Linie lag vorher links
    if (state==0)  state = +2; // Linie lag vorher mittig -> Raten
    if (state==+1) state = +2; // Linie lag vorher rechts
    /*
    if (min(bcl,bcr) < 10) {
    if (bcl < bcr) {
    state = -2;
    } else if (bcr < bcl) {
    state = +2;
    } else {
    state = 0;
    }
    }
    */
    }

    switch (color) {
    case 0: led_setall(0,0,0,0); break;
    case 1: led_setall(0,1,1,0); break;
    case 2: led_setall(1,0,0,1); break;
    }
    /*
    switch (state) {
    case -2: led_setall(1,0,0,0); break;
    case -1: led_setall(0,1,0,0); break;
    case  0: led_setall(0,1,1,0); break;
    case +1: led_setall(0,0,1,0); break;
    case +2: led_setall(0,0,0,1); break;
    }
    */
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
        case -2:
            left=-600; right= 900; break;
        case -1:
            left= 600; right= 1000; loose_cnt = 550; break;             // loose_cnt reset
        case  0:
            left= 900-stear; right= 900+stear; loose_cnt = 550; break;  // loose_cnt reset
        case +1:
            left= 1000; right= 600; loose_cnt = 550; break;             // loose_cnt reset
        case +2:
            left= 900; right=-600; break;
    }
    
    // Wenn die Linie verloren wurde, kurz warten und danach eine Zeit
    // lang den Roboter drehen, um die Linie wiederzufinden.
    if (loose_cnt>500) {
    // warten...
    loose_cnt--;
    } else if (loose_cnt) {
    // auf der Stelle drehen...
    loose_cnt--;
    if (state>0) {left=+800; right=-800;}
    if (state<0) {left=-800; right=+800;}
    } else {
    // Linie endgültig verloren :-(
    left = right = 0;
    }
  
    if (ignore_color) {
    ignore_color--;
    } else {
      
    if (color) {
    left /= 2;
    right /= 2;
    }
      
    if (colstabcnt>=8) {
    bot_halt((left+right)/2);
    left=right=0;
    }
    }
  
    uint8_t l  = min(127, analog_getValueExt(ANALOG_FL, 2) /8);
    uint8_t r  = min(127, analog_getValueExt(ANALOG_FR, 2) /8);
    uint8_t ll = min(127, analog_getValueExt(ANALOG_FLL, 2)/8);
    uint8_t rr = min(127, analog_getValueExt(ANALOG_FRR, 2)/8);
  
    obst_max = max4(l,r,ll,rr);
    if (obst_max>8) {
    // Hindernis
    left = right = 0;
    }
  
    // Leistung der Motoren setzen:
    motpwm_setLeft(left);
    motpwm_setRight(right);
	
}
