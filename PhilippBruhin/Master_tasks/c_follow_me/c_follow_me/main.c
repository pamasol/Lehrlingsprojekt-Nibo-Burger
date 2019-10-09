/*
  Follow me - Objektverfolgung für den NIBO burger
  Die vier IR-Sensorbricks sollten in den vorderen Slots (FLL, FL, FR, FRR) stecken!
*/

//#include <niboburger/base.h>
//#include <niboburger/iodefs.h>
//#include <niboburger/usart.h>
//#include <niboburger/motpwm.h>
//#include <niboburger/analog.h>
//#include <niboburger/led.h>
//#include <niboburger/key.h>
//#include <niboburger/delay.h>
//#include <niboburger/utils.h>
#include <niboburger/robomain.h>

#include "maroon.h"


uint8_t progmode = 0;


enum {
  EVENT_NONE              =  0,
  EVENT_KEY1              =  1,
  EVENT_KEY2              =  2,
  EVENT_KEY3              =  3,
  EVENT_TIMEOUT           =  4,
  
  EVENT_NO_HAND           = 20,
  EVENT_TO_CLOSE          = 21,
  EVENT_HAND_C            = 22,
  EVENT_HAND_L            = 23,
  EVENT_HAND_R            = 24,  
  EVENT_HAND_LL           = 25,
  EVENT_HAND_RR           = 26
  
};


uint8_t cops1[8] = {
  0x10,
  0x20,
  0x40,
  0x80,
  0x00,
  0x00,
  0x00,
  0x00
};

uint8_t cops2[8] = {
  0x30,
  0x30,
  0x30,
  0xc0,
  0xc0,
  0xc0,
  0x00,
  0x00
};

uint8_t key_getEvent();
  

void blink_led(uint8_t led, uint8_t count) {
  while (count--) {
    led_set(led, 1);
    delay(80);
    led_set(led, 0);
    delay(120);
  }
}


/* OBSTACLE */

int8_t obstacle_pos;
uint8_t obstacle_val;

uint8_t getObstSensorValue(uint8_t sensor) {
  uint16_t val = analog_getValueExt(sensor, 2);
  if (val&0xff00) return 0xff;
  return val;
}

uint8_t follow_getEvent(uint8_t reset) {
  static uint8_t state = 0;
  uint8_t nstate = 0;
  uint8_t l = getObstSensorValue(ANALOG_FL)>>1;
  uint8_t r = getObstSensorValue(ANALOG_FR)>>1;
  uint8_t ll = getObstSensorValue(ANALOG_FLL)>>1;
  uint8_t rr = getObstSensorValue(ANALOG_FRR)>>1;
  uint8_t cc = l + r;
  ll += l;
  rr += r;
 
  obstacle_val = (ll>rr)?ll:rr;
  if (cc>obstacle_val) {
    obstacle_val = cc;
  }
  
  if ((obstacle_val==cc) || (ll==rr)) {
    if (ll>rr) {
      obstacle_pos = -1;
      nstate = EVENT_HAND_L;
    } else if (ll<rr) {
      obstacle_pos = +1;
      nstate = EVENT_HAND_R;
    } else {
      obstacle_pos = 0;
      nstate = EVENT_HAND_C;
    }
  } else if (ll>rr) {
    obstacle_pos = -2;
    nstate = EVENT_HAND_LL;
  } else {
    obstacle_pos = +2;
    nstate = EVENT_HAND_RR;
  }

  if (obstacle_val>50) {
    nstate = EVENT_TO_CLOSE;
  } else if (obstacle_val<8) {
    nstate = EVENT_NO_HAND;
  }
 
  if (state!=nstate) {
    state = nstate;
    return state;
  }
 
  return EVENT_NONE;
}



/* KEY */

uint8_t key_getEvent() {
  static uint8_t key = 0;
  uint8_t act = key_get_state();
  if (act==0) {
    act = key;
    key = 0;
    if (act==0x01) return EVENT_KEY1;
    if (act==0x02) return EVENT_KEY2;
    if (act==0x04) return EVENT_KEY3;
    return EVENT_NONE;
  }
  if (act!=KEY_STATE_INVALID) {
    key |= act;  
  }
  return EVENT_NONE;
}


uint8_t getEvent() {
  uint8_t event = EVENT_NONE;
  event = key_getEvent();
  if (event) return event;  
  event = follow_getEvent(0);
  return event;
}


uint8_t run = 0;
uint16_t counter = 0;


void handle_event(uint8_t event) {
  uint8_t has_maroon = maroon_connected();
  
  if (run==0) {
    if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
      run = 1;
      follow_getEvent(1);
    }
    return;
  } 
  
  if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
    run = 0;
    motpid_stop(1);
    led_setall(1, 0, 0, 1);
    return;    
  }
     
      
  if (event==EVENT_TO_CLOSE) {
    motpid_stop(1);
    if (has_maroon) led_setall(1, 0, 0, 1);
    if (!has_maroon) led_setall(1, 1, 1, 1);

  } else if (event==EVENT_NO_HAND) {
    motpid_stop(0);
    led_setall(0, 0, 0, 0);

  } else if (event==EVENT_HAND_L) {
    motpid_setSpeed(+20,+30);
    if (has_maroon) led_setall(0, 0, 0, 0);
    if (!has_maroon) led_setall(0, 1, 0, 0);

  } else if (event==EVENT_HAND_C) {
    motpid_setSpeed(+30,+30);
    if (has_maroon) led_setall(0, 0, 0, 0);
    if (!has_maroon) led_setall(0, 1, 1, 0);

  } else if (event==EVENT_HAND_R) {
    motpid_setSpeed(+30,+20);
    if (has_maroon) led_setall(0, 0, 0, 0);
    if (!has_maroon) led_setall(0, 0, 1, 0);

  } else if (event==EVENT_HAND_LL) {
    motpid_setSpeed(-10,+20);
    if (has_maroon) led_setall(0, 0, 0, 0);
    if (!has_maroon) led_setall(1, 1, 0, 0);

  } else if (event==EVENT_HAND_RR) {
    motpid_setSpeed(+20,-10);
    if (has_maroon) led_setall(0, 0, 0, 0);
    if (!has_maroon) led_setall(0, 0, 1, 1);
      
  }          

}



void setup() {
  activate_output_group(IO_LEDS);  // LED bits als Output
  
  maroon_setup();
  motpwm_init();
  motpid_init();
  odometry_init();
  analog_init();
  
  while (key_getEvent()==EVENT_NONE) {
    // wait...
  }
  
  delay(100);
  delay(100);
  
  maroon_welcome();
  
  motpid_stop(1);
  blink_led(1, 4);
  run = 1;
}


void loop() {
  nibo_checkMonitorVoltage();
  analog_wait_update();
  maroon_loop();
  uint8_t event = getEvent();
  handle_event(event);
}
