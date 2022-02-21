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

#include "pxbuf.h"



uint8_t pxbuf_shiftout(uint8_t * buf, uint8_t mode) {
  uint8_t tmp = 0;
  uint8_t res = 0;
  
  if (mode <= PXBUF_RIGHT_DESC) {
  
    if ((mode & 0x02)==0) {
      // PXBUF_LEFT_ASC + PXBUF_LEFT_DESC
      uint8_t cnt = 7;
      uint8_t * bufw = buf;
      __asm__ volatile(
                 "ld  %[tmp], %a[buf]+ \n\t"
        "L_%=: " "ld  __tmp_reg__, %a[buf]+ \n\t"
                 "st  %a[bufw]+, __tmp_reg__ \n\t"
                 "dec %[cnt]  \n\t"
                 "brne L_%= \n\t"
        : [tmp] "=&r" (tmp)
        : [cnt] "r" (cnt),
          [buf] "z" (buf),
          [bufw] "y" (bufw)
      );
    } else {
      // PXBUF_RIGHT_ASC + PXBUF_RIGHT_DESC
      buf += 8;
      uint8_t cnt = 7;
      uint8_t * bufw = buf;
      __asm__ volatile(
                 "ld  %[tmp], -%a[buf] \n\t"
        "L_%=: " "ld  __tmp_reg__, -%a[buf] \n\t"
                 "st  -%a[bufw], __tmp_reg__ \n\t"
                 "dec %[cnt]  \n\t"
                 "brne L_%= \n\t"
        : [tmp] "=&r" (tmp)
        : [cnt] "r" (cnt),
          [buf] "z" (buf),
          [bufw] "y" (bufw)
      );    
    }
  
    if ((mode&0x01)==0x01) {
      // PXBUF_LEFT_DESC + PXBUF_RIGHT_DESC
      __asm__ volatile(
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
                 "rol %[in] \n\t"
                 "ror %[out] \n\t"
        : [out] "=&r" (res)
        : [in] "r" (tmp)
      );
    } else {
      // PXBUF_LEFT_ASC + PXBUF_RIGHT_ASC
      res = tmp;
    }
    
  } else if (mode==PXBUF_UP_ASC) {
    // TODO
  } else if (mode==PXBUF_UP_DESC) {
    // TODO
  } else if (mode==PXBUF_DOWN_ASC) {
    // TODO
  } else if (mode==PXBUF_DOWN_DESC) {
    // TODO
  }
  
  return res;
}


