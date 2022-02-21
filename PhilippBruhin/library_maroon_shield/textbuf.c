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
#include "textbuf.h"


uint8_t textbuf_buf[TEXTBUF_BUF_SIZE];

volatile uint8_t textbuf_buf_begin;
volatile uint8_t textbuf_buf_end;


char textbuf_getchar() {
  uint8_t result = textbuf_buf[textbuf_buf_begin];
  if (textbuf_buf_end==TEXTBUF_BUF_SIZE) {
    textbuf_buf_end=textbuf_buf_begin;
  }
  
  if (++textbuf_buf_begin>=TEXTBUF_BUF_SIZE) {
    textbuf_buf_begin=0;
  }
  return result;
}



char textbuf_putchar(char c) {
  textbuf_buf[textbuf_buf_end] = c;
  if (++textbuf_buf_end>=TEXTBUF_BUF_SIZE) {
    textbuf_buf_end=0;
  }
  if (textbuf_buf_end==textbuf_buf_begin) {
    textbuf_buf_end=TEXTBUF_BUF_SIZE;
  }
  return c;
}

void textbuf_clear() {
  textbuf_buf_begin=textbuf_buf_end=0;
}
