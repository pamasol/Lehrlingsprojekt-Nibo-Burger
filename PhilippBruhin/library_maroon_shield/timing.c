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
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <util/delay.h>


#include "timing.h"
#include "matrix.h"
#include "shifter.h"
#include "pxbuf.h"
#include "unicode.h"
#include "process.h"

/*
  Timer is set up for 8kHz
  PWM divides by 10
  Multiplexing divides by 8
  -> Display frequency: 100 Hz
*/

uint8_t tcnt=0;


void setup_timing() {
  // 8kHz
  TCCR0A = _BV(WGM01); // CTC
  TCCR0B = _BV(CS01); // PS=8 
  OCR0A = 125-1;
  TIMSK0 = _BV(OCIE0A);  
}


ISR (TIMER0_COMPA_vect) {
  matrix_work_irq();  // will be called 80 times to display one single frame...
  if (++tcnt>=8) {
    tcnt=0;
  }
  process_work_irq(tcnt);
  shifter_work_irq();
}


