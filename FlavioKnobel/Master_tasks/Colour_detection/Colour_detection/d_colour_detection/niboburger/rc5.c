/*  BSD-License:

Copyright (c) 2011 by Nils Springob, nicai-systems, Germany

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

/*!
 *  @file    rc5.c
 *  @brief   Routinen für die Dekodierung von RC5-Fernbedienungs-Codes
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2011-06-05
 */

// Infos:
// http://www.sbprojects.com/knowledge/ir/rc5.htm
// http://www.sprut.de/electronic/ir/rc5.htm

// ========================================================================
// RC5 Infrarot-Empfänger
// ========================================================================

#include <avr/interrupt.h>
#include "niboburger/iodefs.h"
#include "niboburger/analog.h"
#include "niboburger/rc5.h"

/*! IR_RX */
#define IO_IR_RX_PORT PORTD
#define IO_IR_RX_PIN  PIND
#define IO_IR_RX_BIT  1
#define IO_IR_RX_DDR  DDRD

/*! IR_TX0 */
#define IO_IR_TX0_PORT PORTC
#define IO_IR_TX0_PIN  PINC
#define IO_IR_TX0_BIT  2
#define IO_IR_TX0_DDR  DDRC

/*! IR_TX1 */
#define IO_IR_TX1_PORT PORTC
#define IO_IR_TX1_PIN  PINC
#define IO_IR_TX1_BIT  3
#define IO_IR_TX1_DDR  DDRC

#define IR_FREQ 144000

/* tool macros */
#define nop()  __asm__ __volatile__ ("nop" ::)
#define schedule_irq() do {sei();nop();cli();} while(0)

// -----------------------------------------------------------------------------
// Timer 0
// CTC mode
// PS: 1 (TX) / 8 (RX)
// TOP: 111
// => 144.144 kHz (4 * 36 kHz)
// -----------------------------------------------------------------------------
#if defined (TCCR0A)
// ATmega 164, 324, 644, 1284
# define IR_SIGNAL_OC        TIMER0_COMPA_vect
# define IR_TIMER_CNT        TCNT0
# define IR_TIMER_CCRA        TCCR0A
# define IR_TIMER_CCRA_INIT   (_BV(WGM01))
# define IR_TIMER_CCR        TCCR0B
# define IR_TIMER_CCR_RX     (_BV(CS01))
# define IR_TIMER_CCR_TX     (_BV(CS00))
# define IR_TIMER_OCR        OCR0A
# define IR_TIMER_TIMSK      TIMSK0
# define IR_TIMER_TIMSK_INIT _BV(OCIE0A)
#else
// ATmega 16, 32, 64
# define IR_SIGNAL_OC        TIMER0_COMP_vect
# define IR_TIMER_CNT        TCNT0
# define IR_TIMER_CCR        TCCR0
# define IR_TIMER_CCR_RX     (_BV(WGM01)|_BV(CS01))
# define IR_TIMER_CCR_TX     (_BV(WGM01)|_BV(CS00))
# define IR_TIMER_OCR        OCR0
# define IR_TIMER_TIMSK      TIMSK
# define IR_TIMER_TIMSK_INIT _BV(OCIE0)
#endif

#define IR_TIMER_OCR_RX     103
#define IR_TIMER_OCR_TX     103


// -----------------------------------------------------------------------------
// Timing
// -----------------------------------------------------------------------------
#define IR_COUNTVAL_PULSE        128/8
#define IR_COUNTVAL_DELTA        (IR_COUNTVAL_PULSE/4)
#define IR_COUNTVAL_SHORT_MIN    (IR_COUNTVAL_PULSE-IR_COUNTVAL_DELTA)
#define IR_COUNTVAL_SHORT_MAX    (IR_COUNTVAL_PULSE+IR_COUNTVAL_DELTA)
#define IR_COUNTVAL_LONG_MIN     (2*IR_COUNTVAL_PULSE-IR_COUNTVAL_DELTA)
#define IR_COUNTVAL_LONG_MAX     (2*IR_COUNTVAL_PULSE+IR_COUNTVAL_DELTA)
#define IR_COUNTVAL_SILENCE_MIN  (90*IR_COUNTVAL_PULSE)

enum {
  RC5_RX_ENABLE = 0x01,
  RC5_TX_ENABLE = 0x02
};

enum {
  IR_STATE_SILENCE,
  IR_STATE_IDLE,
  IR_STATE_0,
  IR_STATE_1,
  IR_STATE_H,
  IR_STATE_L
};

enum {
  IR_DATA_INVALID_0,
  IR_DATA_INVALID_1,
  IR_DATA_SHORT_0,
  IR_DATA_SHORT_1,
  IR_DATA_LONG_0,
  IR_DATA_LONG_1
};
  

uint8_t ir_state;
uint16_t ir_data_tmp;
uint16_t ir_data;
uint16_t ir_counter;

uint16_t ir_debug;

volatile uint8_t rc5_flags;

uint8_t ir_tx_pulse;         // 0 ... 128=4*32, Zähler für einzelne Pulse, zählt 144 kHz
uint8_t ir_tx_value;         // Aktuell zu übertragenes codiertes Byte, linksbündig
uint8_t ir_tx_data[4];       // Zu übertragene Bits nach Manchester Codierung (0->10, 1->01)
volatile uint8_t ir_tx_cnt;  // Zu übertragene Anzahl von codierten Bits (RC5: 2*14=28)
uint8_t ir_tx_bitpos;        // Index des aktuell übertragenes Bit
uint8_t ir_tx_bytepos;       // Index des aktuell übertragenes Bytes

uint8_t ir_rx_oldval;


ISR (IR_SIGNAL_OC) {
  // called with 144 kHz (36kHz * 4)

  if (rc5_flags & RC5_TX_ENABLE) {
    IR_TIMER_CCR = IR_TIMER_CCR_TX;
    if (ir_tx_pulse==0) {
      // transmit next encoded bit 
      if (ir_tx_cnt==0) {
        // end of transmission
        set_output_bit(IO_IR_TX0);
        set_output_bit(IO_IR_TX1);
        analog_reenable();
        rc5_flags &= ~RC5_TX_ENABLE;
        return;
      }
      ir_tx_bitpos &= 0x07;
      if (ir_tx_bitpos==0) {
        // next byte
        ir_tx_value = ir_tx_data[ir_tx_bytepos];
        ir_tx_bytepos++;
      } else {
        ir_tx_value <<= 1;
      }
      ir_tx_bitpos++;
      ir_tx_cnt--;
    }
    if ( ((ir_tx_pulse&0x03) == 0x00) && (ir_tx_value&0x80) ) {
      clear_output_bit(IO_IR_TX0);
      clear_output_bit(IO_IR_TX1);
    } else {
      set_output_bit(IO_IR_TX0);
      set_output_bit(IO_IR_TX1);
    }
    ir_tx_pulse++;
    if (ir_tx_pulse==128) {
      ir_tx_pulse=0;
    }
    // no receiving while transmitting...
    return;
  }

  if (rc5_flags & RC5_RX_ENABLE) {
    IR_TIMER_CCR = IR_TIMER_CCR_RX;

    uint8_t val = get_input_bit(IO_IR_RX);
    
    if (ir_counter<IR_COUNTVAL_SILENCE_MIN) {
      if (ir_counter == IR_COUNTVAL_LONG_MAX) {
        // timeout
        ir_state = IR_STATE_SILENCE;
      }
      ir_counter++;
    } else {
      if (val) {
        // minimum silence time reached
        ir_state = IR_STATE_IDLE;
      }
    }
    
    if (val==ir_rx_oldval) {
      return;
    }
    ir_rx_oldval=val;
  
    uint16_t cnt = ir_counter;
    uint8_t data = IR_DATA_INVALID_0;
    if (cnt>IR_COUNTVAL_LONG_MAX) data = IR_DATA_INVALID_0;
    else if (cnt>=IR_COUNTVAL_LONG_MIN) data = IR_DATA_LONG_0;
    else if (cnt>IR_COUNTVAL_SHORT_MAX) data = IR_DATA_INVALID_0;
    else if (cnt>=IR_COUNTVAL_SHORT_MIN) data = IR_DATA_SHORT_0;
    if (val==0) {
      data++;
    }

    ir_counter = 0;
  
    schedule_irq();

    if (ir_state==IR_STATE_SILENCE) {
      // do nothing, restart silence timer
    } else if (ir_state==IR_STATE_1) {
      if (data == IR_DATA_SHORT_0) {
        ir_state = IR_STATE_H;
      } else if (data == IR_DATA_LONG_0) {
        ir_state = IR_STATE_0;
      } else {
        ir_state = IR_STATE_SILENCE;
      }
    } else if (ir_state==IR_STATE_L) {
      if (data == IR_DATA_SHORT_0) {
        ir_state = IR_STATE_0;
      } else {
        ir_state = IR_STATE_SILENCE;
      }
    } else if (ir_state==IR_STATE_IDLE) {
      if (data&0x01) {
        ir_state = IR_STATE_1;
        ir_data_tmp = 0x0000;
      } else {
        ir_state=IR_STATE_SILENCE;
      }
    } else if (ir_state==IR_STATE_0) {
      if (data == IR_DATA_SHORT_1) {
        ir_state = IR_STATE_L;
      } else if (data == IR_DATA_LONG_1) {
        ir_state = IR_STATE_1;
      } else {
        ir_state = IR_STATE_SILENCE;
      }
    } else if (ir_state==IR_STATE_H) {
      if (data == IR_DATA_SHORT_1) {
        ir_state = IR_STATE_1;
      } else {
        ir_state = IR_STATE_SILENCE;
      }
    } else {
      ir_state = IR_STATE_SILENCE;
    }

    schedule_irq();

    if (ir_state==IR_STATE_0) {
      ir_data_tmp <<= 1;
    }
  
    if (ir_state==IR_STATE_1) {
      ir_data_tmp <<= 1;
      ir_data_tmp |= 1;
    }

    if ((ir_data_tmp & 0x3000)==0x3000) {
      ir_data = ir_data_tmp;
      ir_data_tmp = 0x0000;
    }
  }

  if (rc5_flags == 0) {
    IR_TIMER_CCR = 0;
    return;
  }
}

static void ir_tx_reset() {
  uint8_t i;
  ir_tx_cnt = 0;
  for (i=0; i<4; i++) {
    ir_tx_data[i] = 0;
  }
  ir_tx_cnt = 0;
  ir_tx_bitpos = 0x80;
  ir_tx_bytepos = 0;
  ir_tx_pulse = 0;
}

static void ir_tx_putbit(uint8_t val) {
  if (ir_tx_bitpos==0) {
    ir_tx_bitpos = 0x80;
    ir_tx_bytepos++;
  }
  if (val) {
    ir_tx_data[ir_tx_bytepos] += ir_tx_bitpos;
  }
  ir_tx_bitpos >>= 1;
  ir_tx_cnt++;
}


void rc5_tx_encode(uint16_t code) {
  uint8_t lo = LOBYTE(code);
  uint8_t hi = HIBYTE(code) | 0x20; // Startbit
  uint8_t i;
  ir_tx_reset();
  for (i=0; i<6; i++) {
    if (hi&0x20) {
      ir_tx_putbit(0);
      ir_tx_putbit(1);
    } else {
      ir_tx_putbit(1);
      ir_tx_putbit(0);
    }
    hi <<= 1;
  }
  for (i=0; i<8; i++) {
    if (lo&0x80) {
      ir_tx_putbit(0);
      ir_tx_putbit(1);
    } else {
      ir_tx_putbit(1);
      ir_tx_putbit(0);
    }
    lo <<= 1;
  }
  ir_tx_bytepos = 0;
  ir_tx_bitpos = 0;
}


void rc5_tx_start(uint16_t code) {
  cli();
  analog_disable();
  set_output_bit(IO_IR_TX0);
  set_output_bit(IO_IR_TX1);
  activate_output_bit(IO_IR_TX0);
  activate_output_bit(IO_IR_TX1);

  rc5_tx_encode(code);
  
  IR_TIMER_CNT = 0;
#if defined(IR_TIMER_CCRA)
  IR_TIMER_CCRA = IR_TIMER_CCRA_INIT;
#endif
  IR_TIMER_CCR = IR_TIMER_CCR_TX;
  IR_TIMER_OCR = IR_TIMER_OCR_TX;
  IR_TIMER_TIMSK = IR_TIMER_TIMSK_INIT;
  rc5_flags |= RC5_TX_ENABLE;
  sei();
}


void rc5_tx_abort() {
  analog_reenable();
  deactivate_output_bit(IO_IR_TX0);
  deactivate_output_bit(IO_IR_TX1);
  ir_tx_cnt=0;
}


void rc5_rx_enable() {
  rc5_flags |= RC5_RX_ENABLE;
  deactivate_output_bit(IO_IR_RX); // Pin auf Input
  clear_output_bit(IO_IR_RX);      // Pullup aus
  IR_TIMER_CNT = 0;
#if defined(IR_TIMER_CCRA)
  IR_TIMER_CCRA = IR_TIMER_CCRA_INIT;
#endif
  IR_TIMER_CCR = IR_TIMER_CCR_RX;
  IR_TIMER_OCR = IR_TIMER_OCR_RX;
  IR_TIMER_TIMSK = IR_TIMER_TIMSK_INIT;
  // wait for silence
  ir_state = IR_STATE_SILENCE;
}


void rc5_rx_disable() {
  rc5_flags &= ~RC5_RX_ENABLE;
}


uint16_t rc5_rx_read() {
  uint16_t retvalue = ir_data;
  ir_data = 0;
  return retvalue;
}

