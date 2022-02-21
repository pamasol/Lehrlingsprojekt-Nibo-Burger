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

/*! @file    usart.h
 *  @brief   Routinen zur Kommunikation über die serielle Schnittstelle
 *  @author  Nils Springob (springob@nicai-systems.com)
 *  @date    2015-10-08
 */

#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USART_TXBUF_SIZE 8
#define USART_RXBUF_SIZE 64

/*!
 * Setzt neue Baudrate
 */
void usart_setbaudrate(uint16_t baud);

extern uint8_t usart_txbuf[USART_TXBUF_SIZE];
extern uint8_t usart_rxbuf[USART_RXBUF_SIZE];

extern volatile uint8_t usart_txbuf_begin;
extern volatile uint8_t usart_txbuf_end;

extern volatile uint8_t usart_rxbuf_begin;
extern volatile uint8_t usart_rxbuf_end;

/*!
 * Senden und empfangen aktivieren.
 */
void usart_enable();

/*!
 * Senden und empfangen deaktivieren.
 */
void usart_disable();


/*!
 * Gibt ein einzelnes Zeichen aus dem Empfangspuffer zurück.
 * Darf nur aufgerufen werden wenn (usart_rxempty()==false) ist.
 */
char usart_getchar();


/*!
 * Schreibt ein einzelnes Zeichen in den Sendepuffer.
 * Darf nur aufgerufen werden solange (usart_txfull()==false) ist.
 */
char usart_putchar(char c);


/*!
 * Liefert true wenn der Empfangspuffer leer ist.
 */
static inline char usart_rxempty() {
  return usart_rxbuf_begin==usart_rxbuf_end;
}


/*!
 * Liefert true wenn der Sendepuffer leer ist.
 */
static inline char usart_txempty() {
  return usart_txbuf_begin==usart_txbuf_end;
}


/*!
 * Liefert true wenn der Empfangspuffer voll ist.
 */
static inline char usart_rxfull() {
  return usart_rxbuf_end==USART_RXBUF_SIZE;
}


/*!
 * Liefert true wenn der Sendepuffer voll ist.
 */
static inline char usart_txfull() {
  return usart_txbuf_end==USART_TXBUF_SIZE;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBOBEE_USART_H_
