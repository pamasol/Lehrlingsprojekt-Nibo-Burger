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


#include "font.h"
#include "unicode.h"

static uint8_t load_invalid_glyph(uint8_t * pxbuf) {
  return font_load_glyph8(3, pxbuf);
}

static uint8_t load_invalid_glyph_small(uint8_t * pxbuf) {
  return font_load_glyph3_ascii(96, pxbuf);
}

uint8_t unicode_load_glyph_plane0_small(uint16_t unicode, uint8_t * pxbuf, uint8_t mode) {
  if (unicode<0x0020) {
    // control char
    
  } else if (unicode<0x0080) {
    // Basic Latin
    return font_load_glyph3_ascii(unicode - 0x20, pxbuf);
    
  }
  return load_invalid_glyph_small(pxbuf);
}

uint8_t unicode_load_glyph_plane0(uint16_t unicode, uint8_t * pxbuf, uint8_t mode) {
  if (unicode<0x0020) {
    // control char
    
  } else if (unicode<0x0080) {
    // Basic Latin
    return font_load_glyph5(unicode - 0x20, pxbuf, mode);
    
  } else if (unicode<0x00a0) {
    // control char
    
  } else if (unicode<0x0100) {
    // Latin-1 Supplement
    if (unicode==0x00a9) /* (c) */ { 
      return font_load_glyph8(0, pxbuf);
    } else if (unicode==0x00b9) /* ¹ */ { 
      return font_load_glyph3_ascii_sup(17, pxbuf);
    } else if (unicode==0x00b2) /* ² */ { 
      return font_load_glyph3_ascii_sup(18, pxbuf);
    } else if (unicode==0x00b3) /* ³ */ { 
      return font_load_glyph3_ascii_sup(19, pxbuf);
    }
    return font_load_glyph5(unicode - 0x40, pxbuf, mode);
    
  } else if (unicode<0x2070) {
    
  } else if (unicode<0x2080) {
    // superscript numbers
    if (unicode==0x2071) /* i */ { 
      return font_load_glyph3_ascii_sup(73, pxbuf);
    }
    return font_load_glyph3_ascii_sup(unicode-0x2070+16, pxbuf);
    
  } else if (unicode<0x2090) {
    // subscript numbers
    return font_load_glyph3_ascii_sub(unicode-0x2080+16, pxbuf);
    
  } else {
    // special glyphs
    if (unicode==0x2591) /* ░ */ {
      return font_load_glyph8(6, pxbuf);
    } else if (unicode==0x2592) /* ▒ */ {
      return font_load_glyph8(7, pxbuf);
    } else if (unicode==0x2593) /* ▓ */ {
      return font_load_glyph8(8, pxbuf);
    } else if (unicode==0x2639) /* ☹ */ {
      return font_load_glyph8(2, pxbuf);      
    } else if ((unicode>=0x2680)&&(unicode<=0x2685)) /* ⚀ - ⚅ */ {
      return font_load_glyph8(9+unicode-0x2680, pxbuf);
    } else if (unicode==0x263A) /* ☺ */ {
      return font_load_glyph8(1, pxbuf);
    } else if (unicode==0x20ac) /* € */ {
      return font_load_glyph5(0xa9-0x40, pxbuf, mode);
    }
  }
  return load_invalid_glyph(pxbuf);
}

uint8_t unicode_load_glyph (uint8_t plane, uint16_t glyph, uint8_t  * pxbuf, uint8_t mode) {
  if (mode & FONT_SMALL) {
    if (plane==0) {
      return unicode_load_glyph_plane0_small(glyph, pxbuf, mode);
    }
    return load_invalid_glyph_small(pxbuf);
  }    
  if (plane==0) {
    return unicode_load_glyph_plane0(glyph, pxbuf, mode);
  }
  return load_invalid_glyph(pxbuf);
}




