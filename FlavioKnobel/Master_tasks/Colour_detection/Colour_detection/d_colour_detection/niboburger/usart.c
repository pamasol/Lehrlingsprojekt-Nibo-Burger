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

#include <avr/interrupt.h>

#include "niboburger/usart.h"
#include "niboburger/iodefs.h"

// for mega644 etc...
#ifndef UBRRH
# define UDR UDR0
# define UBRRH UBRR0H
# define UBRRL UBRR0L
# define UCSRB UCSR0B
# define UDRIE UDRIE0
# define RXCIE RXCIE0
# define RXEN RXEN0
# define TXEN TXEN0
# define USART_RXC_vect USART0_RX_vect
# define USART_UDRE_vect USART0_UDRE_vect
#endif


#ifdef __cplusplus
extern "C" {
#endif


uint8_t usart_txbuf[USART_TXBUF_SIZE];
uint8_t usart_rxbuf[USART_RXBUF_SIZE];

volatile uint8_t usart_txbuf_begin;
volatile uint8_t usart_txbuf_end;

volatile uint8_t usart_rxbuf_begin;
volatile uint8_t usart_rxbuf_end;



void usart_setbaudrate(uint16_t baud) {
  baud = ((uint32_t)(F_CPU))/((uint32_t)baud*16) - 1;
  UBRRH = (uint8_t)(baud>>8);
  UBRRL = (uint8_t)(baud&0xff);
}


char usart_getchar() {
  cli();
  uint8_t result = usart_rxbuf[usart_rxbuf_begin];
  if (usart_rxbuf_end==USART_RXBUF_SIZE) {
    usart_rxbuf_end=usart_rxbuf_begin;
  }
  
  if (++usart_rxbuf_begin>=USART_RXBUF_SIZE) {
    usart_rxbuf_begin=0;
  }
  sei();
  return result;
}


char usart_putchar(char c) {
  cli();
  usart_txbuf[usart_txbuf_end] = c;
  if (++usart_txbuf_end>=USART_TXBUF_SIZE) {
    usart_txbuf_end=0;
  }
  if (usart_txbuf_end==usart_txbuf_begin) {
    usart_txbuf_end=USART_TXBUF_SIZE;
  }
  // reenable transmit register empty interrupt
  UCSRB |= _BV(UDRIE);
  sei();
  return c;
}

void usart_write(const char * s) {
  while (*s) {
    while (usart_txfull()) {}
    usart_putchar (*s++);
  }
}

void usart_write_P(PGM_P s) {
  char c;
  while ((c=pgm_read_byte(s++))) {
    while (usart_txfull()) {}
    usart_putchar (c);
  }
}

void usart_send(const char * s, unsigned int len) {
  while (len--) {
    while (usart_txfull()) {}
    usart_putchar (*s++);
  }
}

void usart_send_P(PGM_P s, unsigned int len) {
  while (len--) {
    while (usart_txfull()) {}
    usart_putchar (pgm_read_byte(s++));
  }
}


static char usart_tx_getchar() {
  uint8_t result = usart_txbuf[usart_txbuf_begin];
  if (usart_txbuf_end==USART_TXBUF_SIZE) {
    usart_txbuf_end=usart_txbuf_begin;
  }
  
  if (++usart_txbuf_begin>=USART_TXBUF_SIZE) {
    usart_txbuf_begin=0;
  }
  return result;
}


static char usart_rx_putchar(char c) {
  usart_rxbuf[usart_rxbuf_end] = c;
  if (++usart_rxbuf_end>=USART_RXBUF_SIZE) {
    usart_rxbuf_end=0;
  }
  if (usart_rxbuf_end==usart_rxbuf_begin) {
    usart_rxbuf_end=USART_RXBUF_SIZE;
  }
  return c;
}


void usart_enable() {
  UCSRB = _BV(RXCIE) + _BV(UDRIE) + _BV(RXEN) + _BV(TXEN);
}


void usart_disable() {
  UCSRB = 0;
}


ISR(USART_RXC_vect) {
  uint8_t c = UDR;
  if (!usart_rxfull()) {
    usart_rx_putchar(c);
  }
}


ISR(USART_UDRE_vect) {
  if (!usart_txempty()) {
    UDR = usart_tx_getchar();
  } else {
    UCSRB &= ~_BV(UDRIE);
  }
}


#ifdef __cplusplus
} // extern "C"
#endif


