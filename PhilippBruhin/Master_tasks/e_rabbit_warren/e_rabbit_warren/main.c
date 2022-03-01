/**
 *	NIBO Burger – Pamasol electronic project for apprentices
 *	Master task E) Rabbit warren
 *	Setup: IR-Bricks in slots FLL, FL, FR, FRR, maroon shield mounted
 *  Instructions:
 *  1. Switch on robot and click key 3 for calibration
 *		a. Put robot on black area and click key 1. Wait for LEDs blinking 5 times.
 *		b. Put robot on white area and click key 2. Wait for LEDs blinking 5 times.
 *		c. Click key 3 for storing the calibration values and wait for blinking LEDs.
 *  2. Put robot to start position and click...
 *  Worth knowing:
 *  
 *  
 */

#include <niboburger/robomain.h>
#include <stdlib.h>
#include <string.h>

uint8_t dispmode;
volatile int8_t state;

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



void setup() {
	led_init();
	analog_init();
	motpwm_init();
  
	/** ANALOG_BCL and ANALOG_BCR are not activated by default as the
	*  others when surface_init(). They need to be activated separately.
	*  ANALOG_BL:  Floor sensor left
	*  ANALOG_BC:  Floor sensor center
	*  ANALOG_BR:  Floor sensor right
	*  ANALOG_BCL: Floor sensor left  -->  light from middle sensor
	*  ANALOG_BCR: Floor sensor right -->  light from middle sensor
	*/
	surface_init();
	analog_setExtToggleMode(ANALOG_BCL, 1);
	analog_setExtToggleMode(ANALOG_BCR, 1);

	led_blink(0,1,1,0);

	// Waiting for push of a key
	while (1) {
		char c = key_get_char();
		if (c=='c') {
			calibrate();
			dispmode=2;
			break;
		} else if (c=='a') {
			dispmode=1;
			break;
		} else if (c=='b') {
			dispmode=2;
			break;
		}
	}
	
}




void loop() {
	nibo_checkMonitorVoltage();
	analog_wait_update();

	int16_t left=0, right=0;

	// Geschwindigkeiten in Abhängigkeit vom aktuellen Linienstatus setzen
	switch (state) {
		case -2:
			left=-600; right= 900; break;
		case -1:
			left= 600; right= 1000; loose_cnt = 550; break; // loose_cnt zurücksetzen
		case  0:
			left= 900-stear; right= 900+stear; loose_cnt = 550; break; // loose_cnt zurücksetzen
		case +1:
			left= 1000; right= 600; loose_cnt = 550; break; // loose_cnt zurücksetzen
		case +2:
			left= 900; right=-600; break;
	}
	
}
