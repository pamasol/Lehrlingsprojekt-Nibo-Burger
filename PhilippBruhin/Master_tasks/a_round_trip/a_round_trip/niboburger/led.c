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

/*! @file    led.c
 *  @brief   Routinen zur Ansteuerung der LEDs
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-06-06
 */

#include <util/atomic.h>
#include "niboburger/iodefs.h"
#include "niboburger/led.h"

#ifdef __cplusplus
extern "C" {
#endif


void led_init() {
  activate_output_group(IO_LEDS);  // LED bits als Output
}

void led_set(uint8_t led, uint8_t status) {
  if (status) {
    switch (led) {
      case 1: set_output_groupbit(IO_LEDS, 1); break;
      case 2: set_output_groupbit(IO_LEDS, 2); break;
      case 3: set_output_groupbit(IO_LEDS, 3); break;
      case 4: set_output_groupbit(IO_LEDS, 4); break;
    }
  } else {
    switch (led) {
      case 1: clear_output_groupbit(IO_LEDS, 1); break;
      case 2: clear_output_groupbit(IO_LEDS, 2); break;
      case 3: clear_output_groupbit(IO_LEDS, 3); break;
      case 4: clear_output_groupbit(IO_LEDS, 4); break;
    }
  }
}

void led_toggle(uint8_t led) {
  switch (led) {
    case 1: toggle_output_groupbit(IO_LEDS, 1); break;
    case 2: toggle_output_groupbit(IO_LEDS, 2); break;
    case 3: toggle_output_groupbit(IO_LEDS, 3); break;
    case 4: toggle_output_groupbit(IO_LEDS, 4); break;
  }
}

uint8_t led_get(uint8_t led) {
  switch (led) {
    case 1: return get_output_groupbit(IO_LEDS, 1)?1:0;
    case 2: return get_output_groupbit(IO_LEDS, 2)?1:0;
    case 3: return get_output_groupbit(IO_LEDS, 3)?1:0;
    case 4: return get_output_groupbit(IO_LEDS, 4)?1:0;
  }
  return 0;
}

void led_setall(uint8_t led1, uint8_t led2, uint8_t led3, uint8_t led4) {
  uint8_t val=0;
  val|=led1?_BV(IO_LEDS_BIT_1):0;
  val|=led2?_BV(IO_LEDS_BIT_2):0;
  val|=led3?_BV(IO_LEDS_BIT_3):0;
  val|=led4?_BV(IO_LEDS_BIT_4):0;

  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
  {
    uint8_t bak = SREG; 
    cli();
    IO_LEDS_PORT = (IO_LEDS_PORT & ~IO_LEDS_MASK) | val;
    SREG = bak;
  }
}


#ifdef __cplusplus
} // extern "C"
#endif

