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

#include "matrix.h"

uint8_t matrix_pixels[8];
uint8_t _matrix_act_col;
uint8_t _matrix_pwm;
uint8_t _matrix_pwm_shadow;
uint8_t _matrix_pwm_cnt;


void setup_matrix() {
  // disable all Col transistors, setup outputs
  PORTB &= ~0xc7; // Col 1-3, Col 7-8
  PORTC &= ~0x38; // Col 4-6
  DDRB |= 0xc7; // Col 1-3, Col 7-8
  DDRC |= 0x38; // Col 4-6

  // switch off all pixels, setup outputs
  PORTC &= ~0x03; // Row 1-2
  PORTD &= ~0xfc; // Row 3-8
  DDRC |= 0x03; // Row 1-2
  DDRD |= 0xfc; // Row 3-8  
  _matrix_act_col = 7;
  _matrix_pwm = _matrix_pwm_shadow = 5;
  
  /*
  matrix_pixels[0] = 0x3c;
  matrix_pixels[1] = 0x42;
  matrix_pixels[2] = 0xa9;
  matrix_pixels[3] = 0x85;
  matrix_pixels[4] = 0x85;
  matrix_pixels[5] = 0xa9;
  matrix_pixels[6] = 0x42;
  matrix_pixels[7] = 0x3c;
  */
}

