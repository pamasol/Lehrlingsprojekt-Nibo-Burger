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
#include <avr/pgmspace.h>

#include <stdio.h>

#include "timing.h"
#include "matrix.h"
#include "shifter.h"
#include "pxbuf.h"
#include "unicode.h"
#include "process.h"
#include "textbuf.h"
#include "font.h"
#include "comm.h"
#include "usart.h"
#include "analog.h"

#define ASCII_ESC 0x1b
#define ASCII_DLE 0x10

uint8_t data_a[8];

uint8_t shift_cnt;
uint8_t space_cnt;
uint8_t space_reload;
uint8_t shift_time;
uint8_t shift_time_reload;

uint8_t comm_lock;

uint8_t font_mode;

uint8_t utf8_state;
uint8_t utf8_a;
uint8_t utf8_b;
uint8_t utf8_c;
uint8_t utf8_d;

uint8_t esc;            // first char after [esc]
uint8_t dle;            // first char after [dle]
uint32_t esc_parameter; // esc sequence parameter

uint8_t flush;          // shift n cols without delay
uint8_t buf_load;       // load data...
uint16_t dimm_reload;   // reload timing for dimmer
uint16_t dimm_count;    // reload timing for dimmer

const uint8_t PROGMEM bargraph[] = {
  /* '1'-'8' */ 1,2,4,8,16,32,64,128,
  /* 'b'-'p' */ 1,3,7,15,31,63,127,255,254,252,248,240,224,192,128,
  /* 'B'-'H' */ 0x18, 0x3c, 0x7e, 0xff, 0xe7, 0xc3, 0x81,
  /* 'I'-'M' */ 0x00, 0x18, 0x24, 0x42, 0x81
};

static uint8_t getBargraphCol(char c) {
  if (c<'1') {    
    return 0x00; // '0' and invalid
  } else if (c<='8') {
    return pgm_read_byte_near(bargraph + c - '1');
  } else if (c<'B') {
    return 0x00; // 'A' and invalid
  } else if (c<='M') {
    return pgm_read_byte_near(bargraph + c - 'B' + 23);
  } else if (c<'b') {
    return 0x00; // 'N', 'a' and invalid
  } else if (c<='p') {
    return pgm_read_byte_near(bargraph + c - 'b' + 8);
  } else {
    return 0x00; // 'q' and invalid
  }
}

static void process_keys() {
  static uint8_t last_key = KEY_NONE;
  uint8_t key = analog_get_value(ANALOG_CH_KEY);
  if (key==KEY_INVALID) {
    return;
  }
  if (key!=last_key) {
    uint8_t xkey = last_key ^ key;
    // KEY EVENT
    if (xkey & KEY_FN) {
      if (key & KEY_FN) {
        usart_putchar('A');
      } else {
        usart_putchar('a');
      }
    }
    if (xkey & KEY_ENTER) {
      if (key & KEY_ENTER) {
        usart_putchar('B');
      } else {
        usart_putchar('b');
      }
    }    
    last_key = key;
  }
}



void printText_P(PGM_P s) {
  char c;
  while ((c = pgm_read_byte(s++))) {
    textbuf_putchar(c);
  }
}

void printText(const char * s) {
  char c;
  while ((c = *s++)) {
    textbuf_putchar(c);
  }
}


static void process_early() {
  // read from uart buffer, process DLE sequences, store in text buffer
  while (!comm_lock && !usart_rxempty() && !textbuf_full()) {
    uint8_t c = usart_getchar();
    
    if (dle==1) {
      // first dle char
      switch (c) {
        // with parameter
        case 'd':
        case 'o':
          dle=c;
          break;

        // direct commands
        case 'c':
          // clear buffer and char processing...
          textbuf_clear();
          utf8_state = 0;
          space_cnt = shift_cnt = 0;
          dle = esc = 0;
          textbuf_putchar(ASCII_ESC);
          textbuf_putchar('c');
          break;
  
        default:
          dle=0;
      }
    } else if (dle) {
      switch (dle) {
        case 'd':
          matrix_set_pwm((c=='*')?10:(c-'0'));
          break;
        case 'o':
          //TODO
          break;
      }
      dle=0;
    } else if (c==ASCII_DLE) {
      dle=1;
    } else {
      textbuf_putchar(c);
    }
  }  
}

static uint8_t parse_hex(uint8_t c1, uint8_t c0) {
  c0 |= 0x20; // 'A' -> 'a'
  c1 |= 0x20; // 'A' -> 'a'
  uint8_t val = (c1&0x40)?(c1-'a'+10):(c1-'0');
  val = (val&0x0f) << 4;
  val += (c0&0x40)?(c0-'a'+10):(c0-'0');
  return val;
}

static void process_stream() {
  // read from text buffer, process ESC sequences, store in unicode char buffer
  if (!textbuf_empty() && ((utf8_state==0)||(utf8_state&0x80))) {
    uint8_t c = textbuf_getchar();
    if (c == ASCII_ESC) {
      esc = c;
      utf8_state = 0;
    } else if (esc) {
      // in escape mode
      if (esc==ASCII_ESC) {
        switch (c) {
          case 'c':
          case 'r':
          case 's':
          case 'l':
            /* no parameter */
            utf8_d = ASCII_ESC;
            utf8_state = 1;
            // fall trought!!
          case 'b':
          case 'd':
          case 'e':
          case 'i':
          case 'o':
          case 'p':
          case 't':
          case 'B':
          case 'D':
          case 'S':
          case 'G':
          case 'P':
          case 'X':
            /* with parameter */
            esc_parameter = 0;
            esc = c;
            utf8_c = 0;
            buf_load = 0;
            break;
          default:
            esc = 0;
        }
      } else {
        if ((esc >= 'a') && (esc <= 'z')) {
          utf8_c = c;
          utf8_d = ASCII_ESC;
          utf8_state = 1;  
        } else {
          if (c==' ') {
            if (esc=='G') {
              esc=' ';
            }
            if (esc=='B') {
              esc=' ';
            }
            utf8_d = ASCII_ESC;
            utf8_state = 1;
          } else {
            if (esc=='X') {
              // hexadecimal parameter
              esc_parameter *= 16;
              c |= 0x20; // 'A' -> 'a'
              esc_parameter += (c&0x40)?(c-'a'+10):(c-'0');
            } if (esc=='G') {
              if (utf8_c) {
                utf8_b = c;
                utf8_d = ASCII_ESC;
                utf8_state = 1;
              } else {
                utf8_c = c;
              }
            } if (esc=='B') {
              utf8_c = c;
              utf8_d = ASCII_ESC;
              utf8_state = 1;
            } else {
              // decimal parameter
              esc_parameter *= 10;
              esc_parameter += c-'0';
            }
          }
        }
      }
      
    } else if (c < UTF8_ASCII_END) {
      utf8_d = c;
      utf8_state = 1;
    } else if (c < UTF8_TAIL_END) {
      uint8_t us = utf8_state & 0xf0;
      if (us == 0xa0) {
        utf8_b = c;
        utf8_state -= 0x10;
      } else if (us == 0x90) {
        utf8_c = c;
        utf8_state -= 0x10;
      } else if (us == 0x80) {
        utf8_d = c;
        utf8_state -= 0x80;
      } else {
        utf8_state = 0;
      }
    } else if (c < UTF8_HEAD2_END) {
      utf8_c = c;
      utf8_state = 0x82;      
    } else if (c < UTF8_HEAD3_END) {
      utf8_b = c;
      utf8_state = 0x93;
    } else if (c < UTF8_HEAD4_END) {
      utf8_a = c;
      utf8_state = 0xa4;
    } else {
      utf8_state = 0;
    }
    
  } else if (textbuf_empty() && comm_lock) {
      comm_lock = 0;
  }
}

static uint8_t transition_dimm(unsigned char val) {
  if (dimm_count) {
    dimm_count--;
    return 1;
  }
  uint8_t pwm = matrix_get_pwm();
  if (pwm<val) {
    pwm++;
  }
  if (pwm>val) {
    pwm--;
  }
  matrix_set_pwm(pwm);
  if (pwm==val) {
    return 0;
  }
  dimm_count=dimm_reload;
  return 1;
}

static void process_char() {
  // read unicode char buffer and load glyph in pixel-buffer
  if (shift_cnt+space_cnt==0) {
    uint16_t cp;
    if (utf8_state==1) {      
      utf8_state=0;
      if (utf8_d == ASCII_ESC) {
        switch (esc) {
          case 'b':
            matrix_set_pwm((utf8_c=='*')?10:(utf8_c-'0'));
            break;
    
          case 'd':
            if (transition_dimm((utf8_c=='*')?10:(utf8_c-'0'))) {
              utf8_state = 1;
              return;
            }
            break;
    
          case 'c':
            flush = space_cnt = 8;
            break;
    
          case 'l':
            shift_time = 0;
            flush = 8;
            break;

          case 't':
            usart_putchar(utf8_c);
            break;

          case 'e':
            if (utf8_c=='1') {
              font_mode |= FONT_SMALL;
            } else {
              font_mode &= ~FONT_SMALL;
            }
            break;

          case 'P':
            if (esc_parameter) {
              esc_parameter--;
              utf8_state = 1;
              return;
            }
            break;

          case 'B':
            data_a[buf_load++] = getBargraphCol(utf8_c);
            utf8_c = 0;
            if (flush==0) {
              shift_cnt = buf_load;
              buf_load = 0;
            }
            return;    
    
          case 'D':
            dimm_reload = esc_parameter;
            break;
             
          case 'G':
            data_a[buf_load++] = parse_hex(utf8_c, utf8_b);
            utf8_b = utf8_c = 0;
            if (flush==0) {
              shift_cnt = buf_load;
              buf_load = 0;
            }
            return;    

          case 'S':
            shift_time_reload = esc_parameter;
            break;
        
          case ' ':
            shift_cnt += buf_load;
            buf_load = 0;
        }
        esc = 0;
      } else if ((utf8_d=='\n')||(utf8_d=='\r')) {
        space_cnt = 10;
      } else {
        shift_cnt = unicode_load_glyph (0, utf8_d, data_a, font_mode);
        space_cnt = space_reload;
        if (flush) {
          flush = shift_cnt+space_cnt;
        }
      }
    } else if (utf8_state==2) {
      utf8_state=0;
      cp = decode_utf8_2 (utf8_c, utf8_d);
      shift_cnt = unicode_load_glyph (0, cp, data_a, font_mode);
      space_cnt = space_reload;
      if (flush) {
        flush = shift_cnt+space_cnt;
      }
    } else if (utf8_state==3) {
      utf8_state=0;
      cp = decode_utf8_3 (utf8_b, utf8_c, utf8_d);
      shift_cnt = unicode_load_glyph (0, cp, data_a, font_mode);
      space_cnt = space_reload;
      if (flush) {
        flush = shift_cnt+space_cnt;
      }
    } else if (utf8_state==4) {
      utf8_state=0;      
    }
  }
}

static void process_shift() {
  // shift pixels from pixel-buffer to output matrix
  if (shift_time) {
    shift_time--;
  }
  
  if (shift_time==0) {
    shift_time = shift_time_reload;
    do {
      if (space_cnt) {
        space_cnt--;
        shifter_cmd = SHIFTER_W;
        shifter_data = 0;
      } else if (shift_cnt) {
        shift_cnt--;
        shifter_cmd = SHIFTER_W;
        shifter_data = pxbuf_shiftout(data_a, PXBUF_LEFT_DESC);
      } else {
        return;
      }
      if (flush) { 
        shifter_work_irq();
        flush--;
      }          
    } while (flush); 
  }
}


void setup_process() {
  uint8_t key = analog_get_value(ANALOG_CH_KEY);
  if (key==KEY_FN) {
    _delay_ms(100);
    uint16_t vcc = analog_get_value(ANALOG_CH_RVOLT);
    vcc = (56265U)/vcc; // vcc in 20 mV Schritten
    vcc *= 20; // vcc in Millivolt
    char s[8];
    sprintf(s, "%04i", vcc);
    comm_lock=1;
    printText_P(PSTR("maroon SW 1.2\nVcc="));
    textbuf_putchar(s[0]);
    textbuf_putchar('.');
    textbuf_putchar(s[1]);
    textbuf_putchar(s[2]);
    printText_P(PSTR("V\ndesigned by nicai-systems\n" MAROON_PAUSE(1000)));
  } else if (key==KEY_ENTER) {
    comm_lock=1;
    printText_P(PSTR(MAROON_LOAD() "⚀" MAROON_PAUSE(500)
                     MAROON_LOAD() "⚁" MAROON_PAUSE(500)
		     MAROON_LOAD() "⚂" MAROON_PAUSE(500)
		     MAROON_LOAD() "⚃" MAROON_PAUSE(500)
		     MAROON_LOAD() "⚄" MAROON_PAUSE(500)
		     MAROON_LOAD() "⚅" MAROON_PAUSE(500)
		     MAROON_CLEAR()
		     "E=mc² H₂O " MAROON_FONT_ECO() "abcdefghijklmnopqrstuvwxyzäöü ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ 0123456789" MAROON_FONT_NORM() " \u2070¹²³\u2074\u2075\u2076\u2077\u2078\u2079 ° ÷ × · ☺ ☹ € ©\n" MAROON_PAUSE(1000)));
  } else if (key==(KEY_FN+KEY_ENTER)) {
    comm_lock=1;
    printText_P(PSTR(MAROON_LOAD() MAROON_GFX("aa55aa55aa55aa55") MAROON_PAUSE(2500) MAROON_LOAD() "▒" MAROON_PAUSE(2500) 
                     MAROON_CLEAR() MAROON_STIME(250) MAROON_BAR("abcdefghijklmnopq") MAROON_BAR("01234567876543210") 
		     MAROON_BAR("ABCDEFGHI") "\n" MAROON_STIME(125) MAROON_PAUSE(1000)));
  } else {
    printText_P(PSTR(MAROON_BRIGHT(0) MAROON_LOAD() "H" MAROON_LOAD() "i" MAROON_DIM(*) MAROON_PAUSE(1000) MAROON_DIM(0) MAROON_CLEAR() MAROON_BRIGHT(5)));
  }
  shift_time = shift_time_reload = 125;
  space_reload = 1;
  dimm_reload = 60; // 60 ms per dim step
  font_mode = FONT_PROP_LEFT;
}


void process_work_irq(uint8_t cnt) {
  if (cnt==0) {
    process_shift();   // 1kHz
  } else if (cnt==1) {
    process_keys();    // 1kHz
  } else if (cnt==2) {
    process_early();   // 1kHz
  } else if (cnt==4) {
    process_stream();  // 1kHz    
  } else if (cnt==6) {
    process_char();    // 1kHz
  }
}


