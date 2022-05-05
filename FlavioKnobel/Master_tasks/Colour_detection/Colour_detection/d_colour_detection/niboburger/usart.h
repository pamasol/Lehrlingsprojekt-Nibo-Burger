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

/*! @file    usart.h
 *  @brief   Routinen zur Kommunikation über die serielle Schnittstelle (Slot X13)
 * 
 * <b>Beispiel</b>
 *  @code
void setup() {
  usart_enable();
  usart_setbaudrate(9600);
  usart_write("hello world!");
}
@endcode
 */

#ifndef NIBOBURGER_USART_H_
#define NIBOBURGER_USART_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

  
/*!
 * Größe des Sendepuffers in Byte (Zeichen)
 */
#define USART_TXBUF_SIZE 16


/*!
 * Größe des Empfangspuffers in Byte (Zeichen)
 */
#define USART_RXBUF_SIZE 8


#ifndef DOXYGEN
extern uint8_t usart_txbuf[USART_TXBUF_SIZE];
extern uint8_t usart_rxbuf[USART_RXBUF_SIZE];

extern volatile uint8_t usart_txbuf_begin;
extern volatile uint8_t usart_txbuf_end;

extern volatile uint8_t usart_rxbuf_begin;
extern volatile uint8_t usart_rxbuf_end;
#endif
  
  
/*!
 * Setzt neue Baudrate
 * 
 * @code usart_setbaudrate(9600); @endcode
 */
void usart_setbaudrate(uint16_t baud);


/*!
 * Senden und Empfangen aktivieren.
 */
void usart_enable();


/*!
 * Senden und Empfangen deaktivieren.
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
 * Schreibt einen null-terminierten String aus dem RAM in den Sendepuffer.
 * Wartet automatisch falls der Sendepuffer voll ist.
 * @param s Pointer zum Null-terminierten Text (im RAM)
 *
 * @code usart_write("hello world!"); @endcode
 */
void usart_write(const char * s);


/*!
 * Schreibt einen null-terminierten String aus dem FLASH in den Sendepuffer.
 * Wartet automatisch falls der Sendepuffer voll ist.
 * @param s Pointer zum Null-terminierten Text (direkt im Flash)
 *
 * @code usart_write_P(PSTR("hello world!")); @endcode
 */
void usart_write_P(PGM_P s);


/*!
 * Schreibt einen Char-Array aus dem RAM mit der angegebenen Länge in den Sendepuffer.
 * Wartet automatisch falls der Sendepuffer voll ist.
 * @param s Pointer zum Text (im RAM)
 * @param len Anzahl der Zeichen
 *
 * @code usart_send("hello world!", 5); // sendet die ersten 5 Zeichen "hello" @endcode
 */
void usart_send(const char * s, unsigned int len);


/*!
 * Schreibt einen Char-Array aus dem FLASH mit der angegebenen Länge in den Sendepuffer.
 * Wartet automatisch falls der Sendepuffer voll ist.
 * @param s Pointer zum Text (direkt im Flash)
 * @param len Anzahl der Zeichen
 *
 * @code usart_send_P(PSTR("hello world!"), 5); // sendet die ersten 5 Zeichen "hello" @endcode
 */
void usart_send_P(PGM_P s, unsigned int len);



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


/*!
 * Liefert Anzahl der bereitstehenden Zeichen im Empfangspuffer.
 */
static inline char usart_rxavail() {
  // fn is isr save (order important)!
  int s=(int)usart_rxbuf_end-(int)usart_rxbuf_begin;
  if (usart_rxfull()) return USART_RXBUF_SIZE;  
  return (s<0)?(USART_RXBUF_SIZE+s):s;
}


/*!
 * Liefert Anzahl der freien Plätze für Zeichen im Sendepuffer.
 */
static inline char usart_txfree() {
  // fn is isr save (order important)!
  int s=(int)usart_txbuf_end-(int)usart_txbuf_begin;
  if (usart_txfull()) return 0;
  return (s<0)?(-s):(USART_TXBUF_SIZE-s);
}


#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBOBURGER_USART_H_
