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

#define UTF8_ASCII_END 0x80
#define UTF8_TAIL_END  0xc0
#define UTF8_HEAD2_END 0xe0
#define UTF8_HEAD3_END 0xf0
#define UTF8_HEAD4_END 0xf5

inline static
uint16_t decode_utf8_2 (uint8_t c, uint8_t d) {  
  uint8_t t = c << 6;
  d = (d&0x3f) | t;
  c = (c&0x1f) >> 2;
  return ((uint16_t)c << 8) | d;
}

inline static
uint16_t decode_utf8_3 (uint8_t b, uint8_t c, uint8_t d) {
  uint8_t t = c << 6;
  d = (d&0x3f) | t;
  b <<= 4;
  c = (c&0x3f) >> 2;
  c = b|c;    
  return ((uint16_t)c << 8) | d;
}

inline static
uint8_t decode_utf8_plane(uint8_t a, uint8_t b) {
  a = (a & 0x07)<<2;
  b = (b & 0x30)>>4;
  return b|a;
}


uint8_t unicode_load_glyph (uint8_t plane, uint16_t glyph, uint8_t  * pxbuf, uint8_t mode);



