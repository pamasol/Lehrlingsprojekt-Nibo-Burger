#include <niboburger/robomain.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "maroon.h"

uint8_t maroon_mode;

void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
}


void maroon_welcome() {
	maroon_mode=0;
	usart_write(MAROON_IMM_CLEAR() MAROON_BRIGHT(0) MAROON_LOAD() "1" MAROON_BAR("0") MAROON_DIM(*) MAROON_PAUSE(100) MAROON_DIM(5) MAROON_STIME(60) " Follow me\n" MAROON_TXBACK("."));
}

void setup() {
	led_init();

  maroon_setup(); 
  delay(100);
  delay(100);
  maroon_welcome();

}

void loop() {
	int ledNr;
	/*
	For loop that does the same process with all LEDs. It does
	not matter if there are 4 or 400, the programming effort
	is the same. Furthermore this code looks cleaner than
	without for statement.
	*/
	for (ledNr=1; ledNr<=4; ledNr++) {
		led_set(ledNr, 1);
		delay(500);
		led_set(ledNr, 0);
		delay(500);
	}
}

