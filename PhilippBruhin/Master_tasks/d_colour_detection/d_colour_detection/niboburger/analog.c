/*  BSD-License:

Copyright (c) 2016 by Nils Springob, nicai-systems, Germany

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

/*! @file    analog.c
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-02-25
 */

#include <avr/interrupt.h>
#include <util/atomic.h>

#include "niboburger/base.h"
#include "niboburger/analog.h"
#include "niboburger/iodefs.h"


#ifdef __cplusplus
extern "C" {
#endif

uint16_t analog_samples[2 * ANALOG_CNT];

uint16_t analog_random_seed;
uint8_t analog_pos;
uint8_t analog_flags;
volatile uint8_t analog_sample_id;

#if defined __AVR_ATmega1284P__ \
         || __AVR_ATmega644__ || __AVR_ATmega644A__ || __AVR_ATmega644P__ || __AVR_ATmega644PA__ \
         || __AVR_ATmega324__ || __AVR_ATmega324A__ || __AVR_ATmega324P__ || __AVR_ATmega324PA__ \
         || __AVR_ATmega164__ || __AVR_ATmega164A__ || __AVR_ATmega164P__
#define ADC_BANDGAP_CHANNEL_VOLTAGE 1.1
//#pragma message ( "ADC_BANDGAP 1.1 V" )
#else
#define ADC_BANDGAP_CHANNEL_VOLTAGE 1.22
//#pragma message ( "ADC_BANDGAP 1.22 V" )
#endif



#define AREF_2V56(x) (_BV(REFS1)|_BV(REFS0)|((x)))
#define AREF_AVCC(x) (_BV(REFS0)|((x)))
#define AREF_EXT(x)  ((x))
#define AMUX_1V22 0x1e
#define AMUX_0V   0x1f

void analog_init() {
#if defined(DIDR0)
  DIDR0 = 0xff;
#endif

  nibo_initialization |= NIBO_ANALOG_INITIALIZED;
  ADCSRA = _BV(ADPS2)  // prescale faktor = 128 ADC laeuft
         | _BV(ADPS1)  // mit 15 MHz / 128 = 120 kHz
         | _BV(ADPS0)
         | _BV(ADEN)   // ADC an
//         | _BV(ADATE)  // auto trigger
         | _BV(ADIE)   // enable interrupt
         | _BV(ADSC);  // Beginne mit der Konvertierung
  activate_output_bit(IO_EN_BL);
  activate_output_bit(IO_EN_BC);
  activate_output_bit(IO_EN_BR);
  activate_output_bit(IO_EN_FL);
  activate_output_bit(IO_EN_FR);
  activate_output_bit(IO_EN_FLL);
  activate_output_bit(IO_EN_FRR);
  analog_flags = 0x7f;

  if (SREG&_BV(7)) {
    // measure all once!
    analog_wait_update();
  } else {
    // precharge Key and Voltage input
    analog_samples[ANALOG_KEY] = analog_samples[ANALOG_KEY+ANALOG_CNT] = 1023;
    analog_samples[ANALOG_VOLT] = analog_samples[ANALOG_VOLT+ANALOG_CNT] = (ADC_BANDGAP_CHANNEL_VOLTAGE*1024)/5;
  }
}

void analog_disable() {
  ADCSRA &= ~_BV(ADIE);
  ADCSRA &= ~_BV(ADEN);
}  

void analog_reenable() {
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADIE);  
}  

uint16_t analog_getValue(uint8_t idx) {
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval = 0;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_samples[idx];
    SREG = bak;
  }
  return retval;
}


uint16_t analog_getValueExt(uint8_t idx, uint8_t active) {
  if (active==0) {
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==1) {
    idx += ANALOG_CNT;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==2) {
    uint8_t idx2 = idx+ANALOG_CNT;
    int16_t val = 0;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx2];
      val -= analog_samples[idx];
      SREG = bak;
    }
    return (val<0)?0:val;
  }
  return 0;
}

uint16_t analog_getRandomSeed() {
  //random_r(unsigned long *  __ctx);
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_random_seed;
    SREG = bak;
  }
  return retval;
}


void analog_wait_update() {
  uint8_t id = analog_sample_id;
  while (id == analog_sample_id) {
  }
}


uint8_t analog_has_update() {
  static uint8_t id = 0;
  if (id == analog_sample_id) {
    return 0;
  }
  id = analog_sample_id;
  return 1;
}


/*                Timing
  All enable signals are low active!

      0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15  MUX
  BL  L+~~~~H- .  .  .  .  .  .  .  .  .  .  .  .  .    5
  RL  .  L+~~~~H- .  .  .  .  .  .  .  .  .  .  .  .    2
  FL  .  .  .  .  L+~~~~H- .  .  .  .  .  .  .  .  .    0
  BC  .  .  .  .  .  L+~~~~H- .  .  .  .  .  .  .  .    6
  FR  .  .  .  .  .  .  .  .  L+~~~~H- .  .  .  .  .    1
  RR  .  .  .  .  .  .  .  .  .  L+~~~~H- .  .  .  .    3
  BR  .  .  .  .  .  .  .  .  .  .  .  .  L+~~~~H- .    7

 KEY  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  *    4
*/


void analog_setExtToggleMode(uint8_t idx, uint8_t activate) {
  uint8_t disable = 0;
  if (idx==8) {
    idx=7;
  }
  if (idx>8) {
    return;
  }

  if (activate) {
    disable = 1;
    analog_flags |= _BV(idx);
    switch (idx) {
      case 0: clear_output_groupbit(IO_ANALOG, SENS_BL); break;
      case 1: clear_output_groupbit(IO_ANALOG, SENS_FLL); break;
      case 2: clear_output_groupbit(IO_ANALOG, SENS_FL); break;
      case 3: clear_output_groupbit(IO_ANALOG, SENS_BC); break;
      case 4: clear_output_groupbit(IO_ANALOG, SENS_FR); break;
      case 5: clear_output_groupbit(IO_ANALOG, SENS_FRR); break;
      case 6: clear_output_groupbit(IO_ANALOG, SENS_BR); break;
      case 7: clear_output_groupbit(IO_ANALOG, SENS_BC); break;
    }
  } else {
    if (analog_flags & _BV(idx)) {
      analog_flags &= ~_BV(idx);
      disable = 1;
    }
  }
  if (disable) {
    switch (idx) {
      case 0: set_output_bit(IO_EN_BL); break;
      case 1: set_output_bit(IO_EN_FLL); break;
      case 2: set_output_bit(IO_EN_FL); break;
      case 3: set_output_bit(IO_EN_BC); break;
      case 4: set_output_bit(IO_EN_FR); break;
      case 5: set_output_bit(IO_EN_FRR); break;
      case 6: set_output_bit(IO_EN_BR); break;
      case 7: set_output_bit(IO_EN_BC); break;
    }
  }
}


void analog_irq_hook() __attribute__((weak));
void analog_irq_hook() {
}

void _key_update_state() __attribute__((weak));
void _key_update_state() {
}

void _key_update_event() __attribute__((weak));
void _key_update_event() {
}

#if F_CPU==15000000
#define STD0 0
#define STD2 2
#define STD3 3
#define STD4 4
#define STD5 5
#define STD6 6

#elif F_CPU==20000000
#define STD0 0
#define STD2 3
#define STD3 4
#define STD4 5
#define STD5 7
#define STD6 8

#else
#error "unsupported clock speed!"
#endif


#define CASE_ADC(CASE, ANALOG_CH, IO_EN_BIT, APORT) \
    case CASE+STD0: \
      ADMUX = AREF_2V56(APORT); \
      DDRA |= _BV(APORT);\
      DDRA &= ~_BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_samples[ANALOG_CH]=value; \
      if (analog_flags&_BV(ANALOG_CH)) clear_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD5: \
      analog_samples[ANALOG_CH+ANALOG_CNT]=value; \
      if (analog_flags&_BV(ANALOG_CH)) set_output_bit(IO_EN_BIT); \
      break;


#define CASE_ADC2(CASE, ANALOG_CH, IO_EN_BIT, APORT) \
    case CASE+STD0: \
      ADMUX = AREF_2V56(APORT); \
      DDRA |= _BV(APORT);\
      DDRA &= ~_BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_samples[ANALOG_CH]=value; \
      if (analog_flags&_BV(7)) clear_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD5: \
      analog_samples[ANALOG_CH+ANALOG_CNT]=value; \
      if (analog_flags&_BV(7)) set_output_bit(IO_EN_BIT); \
      break;




ISR(ADC_vect) {
  uint16_t value = ADC;
  switch (analog_pos) {
    CASE_ADC(0*STD6,  ANALOG_BL,  IO_EN_BL,  IO_ANALOG_BIT_SENS_BL)
    CASE_ADC(1*STD6,  ANALOG_FLL, IO_EN_FLL, IO_ANALOG_BIT_SENS_FLL)
    CASE_ADC(2*STD6, ANALOG_FL,  IO_EN_FL,  IO_ANALOG_BIT_SENS_FL)
    CASE_ADC(3*STD6, ANALOG_BC,  IO_EN_BC,  IO_ANALOG_BIT_SENS_BC)
    CASE_ADC(4*STD6, ANALOG_FR,  IO_EN_FR,  IO_ANALOG_BIT_SENS_FR)
    CASE_ADC(5*STD6, ANALOG_FRR, IO_EN_FRR, IO_ANALOG_BIT_SENS_FRR)
    CASE_ADC(6*STD6, ANALOG_BR,  IO_EN_BR,  IO_ANALOG_BIT_SENS_BR)
    // special surface
    CASE_ADC2(7*STD6, ANALOG_BCL, IO_EN_BC, IO_ANALOG_BIT_SENS_BL)
    CASE_ADC2(8*STD6, ANALOG_BCR, IO_EN_BC, IO_ANALOG_BIT_SENS_BR)

    case 9*STD6:
      ADMUX = AREF_2V56(IO_ANALOG_BIT_KEYBOARD);
      break;
    case 9*STD6+STD2: {
      analog_samples[ANALOG_KEY+ANALOG_CNT] = analog_samples[ANALOG_KEY];
      analog_samples[ANALOG_KEY] = value;
      ADMUX = AREF_AVCC(AMUX_1V22);
    } break;
    case 9*STD6+STD3:
      _key_update_state();
      break;
    case 9*STD6+STD4:
      _key_update_event();
      break;
    case 9*STD6+STD5+STD2: /* wait a little bit longer for Vref stabilization... */
      analog_samples[ANALOG_VOLT+ANALOG_CNT] = analog_samples[ANALOG_VOLT];
      analog_samples[ANALOG_VOLT] = value;
      ADMUX = AREF_2V56(AMUX_0V);
      break;
  }
  if (++analog_pos>(9*STD6+STD5+STD5+STD2)) {
    analog_pos=0;
    analog_irq_hook();
    analog_sample_id++;
    analog_random_seed<<=1;
  }
  analog_random_seed += value;
  ADCSRA |= _BV(ADSC);
}


#ifdef __cplusplus
} // extern "C"
#endif


