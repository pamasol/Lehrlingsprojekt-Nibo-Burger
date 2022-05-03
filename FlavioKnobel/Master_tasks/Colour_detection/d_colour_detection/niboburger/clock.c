/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*! @file    clock.c
 *  @brief   Zeitzählung
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2010-04-24
 */

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include "niboburger/iodefs.h"
#include "niboburger/clock.h"


#ifdef __cplusplus
extern "C" {
#endif

uint16_t clock_us;
volatile uint16_t clock_ms;
volatile uint32_t clock_sec;
volatile uint16_t clock_waitms;
volatile uint8_t clock_10ms;
volatile uint32_t clock_systime_ms;

uint16_t timer_reload;
uint16_t timer_last;


void clock_sec_irq_hook() __attribute__((weak));

void clock_sec_irq_hook() {
}


void clock_hundredth_irq_hook() __attribute__((weak));

void clock_hundredth_irq_hook() {
}


inline static uint16_t clock_get_systime16_isr() {
  return clock_systime_ms & 0xffff;
}


inline static uint16_t clock_get_systime16() {
  uint16_t val;
  ATOMIC_C90_BEGIN()
  val = clock_get_systime16_isr();
  ATOMIC_C90_END()
  return val;
}


void clock_inc_irq(uint8_t ms, uint16_t us) {
  clock_us += us;
  if (clock_us>=1000) {
    clock_us-=1000;
    ms++;
  }
  // us finished!

  clock_systime_ms+=ms;  
  
  uint8_t call10 = 0; 
  clock_10ms+=ms;
  while (clock_10ms>=10) {
    clock_10ms-=10;
    call10 = 1;
  }

  uint8_t call1000 = 0;  
  clock_ms+=ms;
  while (clock_ms>=1000) {
    clock_ms-=1000;
    clock_sec++;
    call1000 = 1;
  }
  
  if (call10) {
    clock_hundredth_irq_hook();
  }
  if (call1000) {
    clock_sec_irq_hook();
  }
}


void clock_sleep(uint16_t ms) {
  ATOMIC_C90_BEGIN()
  ms += clock_get_systime16_isr();
  while (1) {
    uint16_t t2wait = ms - clock_get_systime16_isr();
    if ((t2wait==0) || ((t2wait&0xff00)==0xff00)) {
      break;
    }
    sleep_enable();
    sei();
    sleep_cpu();
    cli();
    sleep_disable();
  }
  ATOMIC_C90_END()
}


void clock_set_timer(uint16_t ms) {
  timer_reload  = ms;
  timer_last = clock_get_systime16();
}


uint16_t clock_get_timer() {
  uint16_t val;
  val = clock_get_systime16();
  val -= timer_last;
  return (val<timer_reload)?(timer_reload - val):0;
}


void clock_wait_timer() {
  uint16_t ms = clock_get_timer();
  clock_sleep(ms);  
  timer_last = clock_get_systime16();
}


uint32_t clock_get_systime_ms() {
  uint32_t res;
  ATOMIC_C90_BEGIN()
  res = clock_systime_ms;
  ATOMIC_C90_END()
  return res;
}


uint16_t clock_elapsed_ms() {
  static uint16_t last;
  uint16_t act = clock_get_systime16();
  uint16_t res = act-last;
  last = act;
  return res;
}


#ifdef __cplusplus
} // extern "C"
#endif

