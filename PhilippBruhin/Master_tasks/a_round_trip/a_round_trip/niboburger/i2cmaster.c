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

#include "niboburger/iodefs.h"
#include "niboburger/base.h"
#include "niboburger/i2cmaster.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NIBO_I2C_TXRX
volatile uint8_t i2c_tx_size;
volatile uint8_t i2c_rx_size;
#endif

volatile uint8_t i2c_state;

volatile uint8_t i2c_size;
volatile uint8_t i2c_pos;

uint8_t i2c_buf[I2C_BUF_SIZE];
volatile uint8_t i2c_last_TWSR;

enum {
  I2C_Idle,
  I2C_Address,
  I2C_Rx,
  I2C_Rx_Ack,
  I2C_Tx,
  I2C_Tx_Ack,
  I2C_Locked
};

////////////////////////////////////////////////////////////////////////////////

static inline void twi_start() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE) | _BV(TWSTA);
}

static inline void twi_send(uint8_t data) {
  TWDR = data;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
}

static inline void twi_ack() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE) | _BV(TWEA);
}

static inline void twi_nak() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
}

static inline void twi_stop() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
}

static inline void twi_abort() {
  TWCR = _BV(TWEN) | _BV(TWINT);
}

////////////////////////////////////////////////////////////////////////////////

uint8_t i2c_start_transmission(void) {
  uint8_t res = 0;
  cli();
  do {
    if (i2c_state == I2C_BUSY) {
      i2c_state = I2C_ERROR+1;
      break;
    }
  
    if (TWCR & _BV(TWSTO)) {
      i2c_state = I2C_ERROR+2;
      break;
    }
    if (TWCR & _BV(TWSTA)) {
      TWCR = 0;
      TWCR = _BV(TWEN);
      i2c_state = I2C_ERROR+8;
      break;
    }
    i2c_state = I2C_BUSY;
#ifdef NIBO_I2C_TXRX
    i2c_size = i2c_tx_size;
#endif
    twi_start();
    res = 1;
  } while(0);
  sei();
  return res;
}

uint8_t i2c_wait_transmission(void) {
  uint16_t i = 50000; // wait up to 50 ms for command to complete
  while (i--) {
    if ((TWCR & _BV(TWIE))||(TWCR & _BV(TWSTO))) {
      _delay_us(1);
    } else {
//      _delay_us(10);
      return 1;
    }
  }
  cli();
  TWCR = 0;
  TWCR = _BV(TWEN);
  if (i2c_state < I2C_ERROR) {
    i2c_state = I2C_ERROR+3;
  }
  sei();
  return 0;
}


uint8_t i2c_status(void) {
  uint8_t res = i2c_state;
  return res;
}


void i2c_init(void) {
  TWCR = 0;
  TWBR = I2C_TWBR_INIT;
  TWDR = 0xff;
  TWCR = _BV(TWEN);
  i2c_state = I2C_IDLE;
  nibo_initialization |= NIBO_I2C_INITIALIZED;
}


ISR(TWI_vect) {
  uint8_t res = TWSR & TW_STATUS_MASK;
  i2c_last_TWSR = res;
  switch (res) {
    case TW_START:
    case TW_REP_START:
      i2c_pos=0;
    case TW_MT_SLA_ACK:
    case TW_MT_DATA_ACK:
      if (i2c_pos<i2c_size) {
        twi_send(i2c_buf[i2c_pos++]);
      } else {
#ifdef NIBO_I2C_TXRX
        if (i2c_rx_size) {
          i2c_size = i2c_rx_size;
          i2c_buf[0] |= 0x01;
          twi_start();
          break;
        }
#endif
        i2c_state = I2C_SUCCESS;
        twi_stop();
      }
      break;
    
    case TW_MR_DATA_ACK:
      i2c_buf[i2c_pos++] = TWDR;
    case TW_MR_SLA_ACK:
      if (i2c_pos+1<i2c_size) {
        // Receive next byte, and send ACK
        twi_ack();
      } else {
        // Receive last byte, and send NAK
        twi_nak();
      }
      break;
    
    case TW_MR_DATA_NACK:
      // Last byte was received
      i2c_buf[i2c_pos] = TWDR;
      i2c_state = I2C_SUCCESS;
      twi_stop();
      break;
      
    case TW_MT_ARB_LOST:
      twi_abort();
      i2c_state = I2C_ERROR+5;
      break;

    case TW_BUS_ERROR:
    case TW_MT_SLA_NACK:
    case TW_MR_SLA_NACK:
    case TW_MT_DATA_NACK:
      twi_stop();
      i2c_state = I2C_ERROR+6;
      break;
    
    default:
      twi_abort();
      i2c_state = I2C_ERROR+7;
  }
}

#ifdef __cplusplus
} // extern "C"
#endif
