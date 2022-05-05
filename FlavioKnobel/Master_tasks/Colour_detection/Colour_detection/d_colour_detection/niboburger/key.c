#include "niboburger/iodefs.h"
#include "niboburger/analog.h"
#include "niboburger/key.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Key State */
uint8_t _key_unstable_cnt;

/* Key Event Buffer */  
#define _KEY_BUF_SIZE 4
uint8_t _key_buf[_KEY_BUF_SIZE];
uint8_t _key_wpos;

uint8_t _key_state;

/* Key Event generation */  
uint8_t _key_last;


static uint8_t _key_calc_state() {
  uint16_t val1;
  uint16_t val2;
  {
      uint8_t bak = SREG; 
      cli();
      val1 = analog_getValue_ISR(ANALOG_KEY);
      val2 = analog_getValueAct_ISR(ANALOG_KEY);
      SREG = bak;
  }
  if (val1>800) {
    return 0x00;
  }
  uint16_t delta = abs(val2 - val1);
  if (delta<4) {
    uint8_t val_h = (val1>>8)&0xff;
    uint8_t val_l = (val1>>0)&0xff;
    
    // optimized version
    switch (val_h) {
      case 0x01: // [256..511]
        if (val_l<0x7a) { // <378
          if (val_l<0x43) { // <323
            return 0x07;
          } else if (val_l<0x5d) { // <349
            return 0x03;
          }
          return 0x05;
        }
        if (val_l<0xc9) { // <457
          return 0x01;
        }
        return 0x06;
      case 0x02: // [512..767]
        if (val_l<0x2c) { // <556
          return 0x06;
        } else if (val_l<0x7c) { // <636
          return 0x02;
        }
        return 0x04;
      case 0x03: // [768..1023]
        if (val_l<0x0b) { // <779
          return 0x04;
        }
        return 0x00;
    }
  }
  return KEY_STATE_INVALID;
}


uint8_t key_get_state() {
  return _key_state;
}


static void _key_put_char(char c) {
  _key_buf[_key_wpos++] = c;
  if (_key_wpos>(_KEY_BUF_SIZE-1)) {
    _key_wpos=0;
  }
}


static uint8_t _key_get_char() {
  uint8_t rpos=_key_wpos;
  do {
    rpos = rpos?(rpos-1):(_KEY_BUF_SIZE-1);
    char c = _key_buf[rpos];
    if (c) {
      _key_buf[rpos] = 0;
      return c;
    }
  } while (rpos!=_key_wpos);
  return 0;
}


char key_get_char() {
  return _key_get_char();
}

void _key_update_state() {
  uint8_t val = _key_calc_state();
  if (val==0) {
    _key_unstable_cnt=3;
    _key_state=0;
  } else if (val==KEY_STATE_INVALID) {
    _key_unstable_cnt=3;
    _key_state=KEY_STATE_INVALID;
  } else if (_key_unstable_cnt) {
    _key_unstable_cnt--;
    _key_state=KEY_STATE_INVALID;
  } else {
    _key_state=val;
  }
}

void _key_update_event() {
  uint8_t key = _key_state;
  if (key==KEY_STATE_INVALID) {
    return;
  }
  uint8_t event = key ^ _key_last;  
  if (event) {
    // a key changed....
    if (event & 0x01) {
      _key_put_char((key & 0x01)?'A':'a');
    }
    if (event & 0x02) {
      _key_put_char((key & 0x02)?'B':'b');
    }
    if (event & 0x04) {
      _key_put_char((key & 0x04)?'C':'c');
    }
    _key_last = key;
  }  
}



#ifdef __cplusplus
} // extern "C"
#endif
