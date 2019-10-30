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

void maroon_setup() {
    
/** For universal asynchronous communication following parameters have to
 *  be in common
 *  1. Transmission speed in baud per second
 *	2. Data length in bits
 *  3. Start/Stop bit
 */
    usart_setbaudrate(38400);
    usart_enable();
}


void maroon_welcome() {
    maroon_mode=0;
    usart_write(MAROON_IMM_CLEAR() MAROON_BRIGHT(0) MAROON_LOAD() "1" MAROON_BAR("0") MAROON_DIM(*) MAROON_PAUSE(100) MAROON_DIM(5) MAROON_STIME(60) " Pamasol\n" MAROON_TXBACK("."));
}

void maroon_loop() {
}

uint8_t maroon_connected() {
    return maroon_mode;
}