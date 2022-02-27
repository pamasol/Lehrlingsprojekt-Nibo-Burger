#include <niboburger/base.h>
#include <niboburger/iodefs.h>
#include <niboburger/usart.h>
#include <niboburger/motpwm.h>
#include <niboburger/motpid.h>
#include <niboburger/analog.h>
#include <niboburger/odometry.h>
#include <niboburger/led.h>
#include <niboburger/key.h>
#include <niboburger/delay.h>
#include <niboburger/surface.h>
#include <niboburger/color.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "maroon.h"

uint16_t maroon_cnt;
uint8_t maroon_mode;

/** maroon_gfxdata[] is filled with "\33l \33B 00000000" as initial value.
 *  The eight zeros are overwritten during operation with the letters a
 *  to h. Where a is just 1 LED on while h is 8 LEDs on. 
 */
char maroon_gfxdata[] = MAROON_LOAD() MAROON_BAR("00000000");

/** Offset 4 array positions for \33l \33B
 *  Array position 0: \33 --> 33 is octal for 27 decimal, what is ESC in Ascii
 *  Array position 1: l
 *  Array position 2: \33
 *  Array position 3: B
 */
#define MAROON_BAR_OFFSET 4

void maroon_bars(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h) {
    if (usart_txempty()) {
        maroon_gfxdata[MAROON_BAR_OFFSET+0] = a;
        maroon_gfxdata[MAROON_BAR_OFFSET+1] = b;
        maroon_gfxdata[MAROON_BAR_OFFSET+2] = c;
        maroon_gfxdata[MAROON_BAR_OFFSET+3] = d;
        maroon_gfxdata[MAROON_BAR_OFFSET+4] = e;
        maroon_gfxdata[MAROON_BAR_OFFSET+5] = f;
        maroon_gfxdata[MAROON_BAR_OFFSET+6] = g;
        maroon_gfxdata[MAROON_BAR_OFFSET+7] = h;
        usart_write(maroon_gfxdata);
    }
}

void maroon_setup() { 
    /** General for universal asynchronous receiver-transmitter (UART)
     *  communication, following parameters have to be in common: 
     *  1. Transmission speed in baud per second
     *	2. Data length in bits
     *  3. Start/Stop bit
     *  Baud rate of maroon shield is 38400, so we need to set same 
     *  value on the robot and activate sending/receiving.
     */
    usart_setbaudrate(38400);
    usart_enable();
}

void maroon_welcome() {
    maroon_mode=0;
    /** Display welcome message "1 Follow me" as scrolling text and
     *  send back "." as soon as procedure has finished.
     */
    usart_write(MAROON_IMM_CLEAR() MAROON_BRIGHT(0) MAROON_LOAD() "1" MAROON_BAR("0") MAROON_DIM(*) MAROON_PAUSE(100) MAROON_DIM(5) MAROON_STIME(60) " Follow me\n" MAROON_TXBACK("."));
}

char getSensorChar(int16_t val) {
    if (val<5) return 'a';
    if (val<10) return 'b';
    if (val<15) return 'c';
    if (val<25) return 'd';
    if (val<35) return 'e';
    if (val<50) return 'f';
    if (val<70) return 'g';
    if (val<100) return 'h';
    return 'i';
}

void maroon_loop() {
    /** Waiting until welcome message has finished and getting back
     *  a "." from the maroon shield micro controller Atmel
     *  ATmega88A.
     */
    if (!usart_rxempty()) {
        char c = usart_getchar();
        if (c=='.') {
            maroon_mode = 1;
        }
    }

    // Update display not every loop. Only every 20th loop.
    if (maroon_mode) {
        if (maroon_cnt) {
            maroon_cnt--;
            return;
        }
        maroon_cnt=20;
        /** If usart_txempty() not true (respectively 1), the controller is
         *  still sending the previous command and therefore it will wait
         *  before sending a new command.
         */        
        if (usart_txempty()) {
            /** analog_getValueExt() sends back values from 0 up to 1024.
             *  getSensorChar() divides the return value into 8 ranges and
             *  assigns letters from a to h.
             */   
            char l = getSensorChar(analog_getValueExt(ANALOG_FL, 2));
            char r = getSensorChar(analog_getValueExt(ANALOG_FR, 2));
            char ll = getSensorChar(analog_getValueExt(ANALOG_FLL, 2));
            char rr = getSensorChar(analog_getValueExt(ANALOG_FRR, 2));
            /** From the 8 rows of maroon shield only 4 are used - the most left
             *  and right as well as the third and 6th row.
             */       
            maroon_bars(rr, '0', r, '0', '0', l, '0', ll);
        }
    }
}

// Check if receiving data via UART. If so, maroon shield mounted.
uint8_t maroon_connected() {
    return maroon_mode;
}
