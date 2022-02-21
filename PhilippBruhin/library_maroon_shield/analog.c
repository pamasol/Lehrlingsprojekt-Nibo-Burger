/*  BSD-License:

Copyright (c) 2015 by Nils Springob, nicai-systems, Germany

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


#include <avr/io.h>
#include "analog.h"
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

#define ANALOG_MASK 0x04


uint8_t idx_act;

uint16_t analog_data[8];

uint16_t lastkey;

inline static uint8_t classify_key(int16_t aval) {
  int16_t delta = aval - lastkey;
  lastkey = aval;
  if (delta > +5) return KEY_INVALID;
  if (delta < -5) return KEY_INVALID;
  
  uint8_t val = aval>>2;
  if (val>160) return KEY_NONE;
  if (val>136) return KEY_INVALID;
  if (val>117) return KEY_ENTER;
  if (val> 94) return KEY_INVALID;
  if (val> 75) return KEY_FN;
  if (val> 72) return KEY_INVALID;
  if (val> 53) return KEY_FN + KEY_ENTER;
  return KEY_INVALID;
}


inline static void select_mux(uint8_t idx) {
  switch (idx) {
    case  0: ADMUX=0x42; break; // single, ADC2 @ AVcc
    case  4: ADMUX=0xc8; break; // single, Temp @ 1.1V
    case  8: ADMUX=0x4e; break; // single, 1.1V @ AVcc
  }
}

void analog_init() {
  PORTC &= ~ANALOG_MASK;
  DIDR0 = ANALOG_MASK;  
  idx_act=0;
  select_mux(idx_act);
  // PS=128 -> ADCCLK=62.5kHz@8Mhz -> 4464 samples/sec -> 744 samples/sec/channel
  // Enable, start free-running, irq mode
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIF) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}


ISR(ADC_vect) {
  select_mux(idx_act);
  uint16_t val = ADC;

  switch (idx_act) {
    case  1: analog_data[0] = val; break;
    case  5: analog_data[1] = classify_key(val); break;
    case  9: analog_data[2] = val; break;
  }
  idx_act++;
  if (idx_act>=12) {
    idx_act=0;
  }
}



int16_t analog_get_value(uint8_t idx) {
  int16_t val;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    val = analog_data[idx];
  }
  return val;
}


