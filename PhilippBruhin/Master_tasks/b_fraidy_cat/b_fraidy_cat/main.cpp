/*
  Fraidy Cat - Hindernisvermeidung für den NIBO burger
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

enum {
  EVENT_NONE              =  0,
  EVENT_KEY1              =  1,
  EVENT_KEY2              =  2,
  EVENT_KEY3              =  3,
  EVENT_TIMEOUT           =  4,
  
  EVENT_OBSTACLE_DETECTED = 16,
  EVENT_OBSTACLE_CLEAR    = 17,
  EVENT_OBSTACLE_CLEAR_L  = 18,
  EVENT_OBSTACLE_CLEAR_R  = 19
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


// auf den Zahlenbereich 0-255 (8 Bit) beschränken
uint8_t getObstSensorValue(uint8_t sensor) {
  uint16_t val = analog_getValueExt(sensor, 2);
  if (val&0xff00) return 0xff;
  return val;
}


uint8_t obstacle_getEvent(uint8_t reset) {
  static uint8_t event_last = EVENT_NONE;
  
  if (reset) {
    event_last = EVENT_NONE;
    return EVENT_NONE;
  }
  
  uint8_t event = EVENT_NONE;
  uint8_t l = getObstSensorValue(ANALOG_FL)>>1;
  uint8_t r = getObstSensorValue(ANALOG_FR)>>1;
  
  uint8_t ll = getObstSensorValue(ANALOG_FLL)>>1;
  uint8_t rr = getObstSensorValue(ANALOG_FRR)>>1;
  uint8_t cc = l + r;
  ll += l;
  rr += r;

  // drei Werte: ll, cc und rr
  
  obstacle_val = max3(ll, cc, rr);
  
  if ((obstacle_val==cc) || (ll==rr)) {
    if (ll>rr) {
      // Hindernis ist leicht links
      obstacle_pos = -1;
    } else if (ll<rr) {
      // Hindernis ist leicht rechts
      obstacle_pos = +1;
    } else {
      // Hindernis ist mittig
      obstacle_pos = 0;
    }
  } else if (ll>rr) {
     // Hindernis ist links
    obstacle_pos = -2;
  } else {
    // Hindernis ist rechts
    obstacle_pos = +2;
  }

  if (obstacle_val>25) {
    event = EVENT_OBSTACLE_DETECTED;
  }
  
  if (obstacle_val<20) {
    event = EVENT_OBSTACLE_CLEAR;
    if (obstacle_val>5) {
      if (obstacle_pos<0) {
        event = EVENT_OBSTACLE_CLEAR_R;
      } else if (obstacle_pos>0) {
        event = EVENT_OBSTACLE_CLEAR_L;
      }
    }
  } 
    
  if (event!=event_last) {
    event_last = event;
    return event;
  }
  
  return EVENT_NONE;
}

/* KEY */

uint8_t key_getEvent() {
  uint8_t c = key_get_char();
  if (c=='a') return EVENT_KEY1;
  if (c=='b') return EVENT_KEY2;
  if (c=='c') return EVENT_KEY3;
  return EVENT_NONE;
}


uint8_t getEvent() {
  uint8_t event = EVENT_NONE;
  event = key_getEvent();
  if (event) return event;  
  event = obstacle_getEvent(0);
  return event;
}


uint8_t run = 0;


void setup() {
  activate_output_group(IO_LEDS);  // LED bits als Output
  
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
  blink_led(2, 4);
  run = 1;
}


void handle_event(uint8_t event) {
  if (run==0) {
    if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
      run = 1;
      obstacle_getEvent(1);
    }
    return;
  } 
  
  if ((event==EVENT_KEY1) || (event==EVENT_KEY2) || (event==EVENT_KEY3)) {
    run = 0;
    motpid_stop(1);
    led_setall(1, 0, 0, 1);
    return;    
  }

  int8_t left = 0;
  int8_t right = 0;

  if (event==EVENT_OBSTACLE_DETECTED) {
    led_setall(obstacle_pos<=0, 0, 0, obstacle_pos>=0);
	if (obstacle_pos>0) {
      motpid_setSpeed(-25,+15);
    } else if (obstacle_pos<0) {
      motpid_setSpeed(+15,-25);
	} else {
      motpid_setSpeed(-20,-20);
	}
  }
      
  if (event==EVENT_OBSTACLE_CLEAR) {
    led_setall(0, 0, 0, 0);
    motpid_setSpeed(+30,+30);
  }

  if (event==EVENT_OBSTACLE_CLEAR_L) {
    led_setall(0, 0, 1, 0);
    motpid_setSpeed(+20,+30);
  }
    
  if (event==EVENT_OBSTACLE_CLEAR_R) {
    led_setall(0, 1, 0, 0);
    motpid_setSpeed(+30,+20);
  }
}


void loop() {
  nibo_checkMonitorVoltage();
  analog_wait_update();
  uint8_t event = getEvent();
  handle_event(event);
}
