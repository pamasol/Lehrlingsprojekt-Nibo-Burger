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

extern uint8_t matrix_pixels[8];
extern uint8_t _matrix_act_col;
extern uint8_t _matrix_pwm;
extern uint8_t _matrix_pwm_shadow;
extern uint8_t _matrix_pwm_cnt;

#define MATRIX_PWM_MAX 10

void setup_matrix();


inline static void matrix_set_pwm(uint8_t val) {
  val = (val<MATRIX_PWM_MAX)?val:MATRIX_PWM_MAX;
  _matrix_pwm_shadow = val;
}

inline static uint8_t matrix_get_pwm() {
  return _matrix_pwm;
}

inline static void _matrix_disable_cols() {
  PORTB &= ~ 0xc7;
  PORTC &= ~ 0x38;
}

inline static void _matrix_select_col(uint8_t col) {
  switch (col) {
    case 0: PORTB |= 0x01; break;
    case 1: PORTB |= 0x02; break;
    case 2: PORTB |= 0x04; break;
    case 3: PORTC |= 0x08; break;
    case 4: PORTC |= 0x10; break;
    case 5: PORTC |= 0x20; break;
    case 6: PORTB |= 0x40; break;
    case 7: PORTB |= 0x80; break;
  }
}

inline static void _matrix_output_row(uint8_t val) {
  PORTC = (PORTC & ~0x03) | (val & 0x03);
  PORTD = (PORTD & ~0xfc) | (val & 0xfc);
}


// called with 8kHz
inline static void matrix_work_irq() {
  _matrix_pwm_cnt++;
  if (_matrix_pwm == 0) {
    //switch off
    _matrix_disable_cols();
    _matrix_output_row(0);
    _matrix_pwm = _matrix_pwm_shadow;
    return;
  }
  if (_matrix_pwm_cnt == _matrix_pwm) {
    //switch off
    _matrix_disable_cols();
  }
  if (_matrix_pwm_cnt >= MATRIX_PWM_MAX) {
    // setup next col
    if (++_matrix_act_col>7) {
      _matrix_act_col=0;
    };
    _matrix_output_row(matrix_pixels[_matrix_act_col]);
    _matrix_select_col(_matrix_act_col);
    _matrix_pwm_cnt=0;
    _matrix_pwm = _matrix_pwm_shadow;
  }
}

