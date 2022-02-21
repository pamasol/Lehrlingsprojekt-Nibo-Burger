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



enum {
  SHIFTER_NONE,
  SHIFTER_N,
  SHIFTER_W,
  SHIFTER_E,
  SHIFTER_S
};



extern volatile int8_t shifter_cmd;
extern volatile int8_t shifter_data;

inline static void shifter_work_irq() {
  uint8_t cmd = shifter_cmd;
  uint8_t data = shifter_data;
  if (cmd==SHIFTER_NONE) {
    return;
    
  } else if (cmd==SHIFTER_N) {
    uint8_t cnt=8;
    __asm__(
      "L_%=: " "rol %[data] \n\t"
               "ld  __tmp_reg__, %a[arr] \n\t"
               "rol __tmp_reg__ \n\t"
               "st  %a[arr]+, __tmp_reg__ \n\t"
               "dec %[cnt]  \n\t"
               "brne L_%= \n\t"
      : 
      : [cnt] "r" (cnt), 
        [data] "r" (data), 
        [arr]  "e" (matrix_pixels)
    );
    
    /*
    matrix_pixels[0] <<= 1;
    matrix_pixels[1] <<= 1;
    matrix_pixels[2] <<= 1;
    matrix_pixels[3] <<= 1;
    matrix_pixels[4] <<= 1;
    matrix_pixels[5] <<= 1;
    matrix_pixels[6] <<= 1;
    matrix_pixels[7] <<= 1;
    */
    
  } else if (cmd==SHIFTER_W) {
    matrix_pixels[7] = matrix_pixels[6];
    matrix_pixels[6] = matrix_pixels[5];
    matrix_pixels[5] = matrix_pixels[4];
    matrix_pixels[4] = matrix_pixels[3];
    matrix_pixels[3] = matrix_pixels[2];
    matrix_pixels[2] = matrix_pixels[1];
    matrix_pixels[1] = matrix_pixels[0];
    matrix_pixels[0] = data;
    
  } else if (cmd==SHIFTER_E) {
    matrix_pixels[0] = matrix_pixels[1];
    matrix_pixels[1] = matrix_pixels[2];
    matrix_pixels[2] = matrix_pixels[3];
    matrix_pixels[3] = matrix_pixels[4];
    matrix_pixels[4] = matrix_pixels[5];
    matrix_pixels[5] = matrix_pixels[6];
    matrix_pixels[6] = matrix_pixels[7];
    matrix_pixels[7] = data;
    
  } else if (cmd==SHIFTER_S) {
    uint8_t cnt=8;
    __asm__(
      "L_%=: " "rol %[data] \n\t"
               "ld  __tmp_reg__, %a[arr] \n\t"
               "ror __tmp_reg__ \n\t"
               "st  %a[arr]+, __tmp_reg__ \n\t"
               "dec %[cnt]  \n\t"
               "brne L_%= \n\t"
      : 
      : [cnt] "r" (cnt), 
        [data] "r" (data), 
        [arr]  "e" (matrix_pixels)
    );
    
    /*
    matrix_pixels[0] >>= 1;
    matrix_pixels[1] >>= 1;
    matrix_pixels[2] >>= 1;
    matrix_pixels[3] >>= 1;
    matrix_pixels[4] >>= 1;
    matrix_pixels[5] >>= 1;
    matrix_pixels[6] >>= 1;
    matrix_pixels[7] >>= 1;    
    */
    
  }  
  shifter_cmd = SHIFTER_NONE;
}
