/**
 *	NIBO Burger – Pamasol electronic project for apprentices
 *	Master task B) Fraidy cat
 *	Setup: IR-Bricks in slots FLL, FL, FR, FRR, no maroon shield mounted
 */

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

/** @brief	Makes an LED blink with 80ms on and 120ms off
 *
 *  @param	led		LED number
 *	@param	count	How many times led should blink
 *
 *  @return void
 */
void blink_led(uint8_t led, uint8_t count) {
  while (count--) {
    led_set(led, 1);
    delay(80);
    led_set(led, 0);
    delay(120);
  }
}


/************************************************************************/
/* OBSTRACLE                                                            */
/************************************************************************/

int8_t obstacle_pos;
uint8_t obstacle_val;

/** @brief	Limit number range to 0-255 (8 bit) 
 *
 *  @param	sensor	Sensor number
 *	
 *	@return	val		Number between 0 and 255
 */
uint8_t getObstSensorValue(uint8_t sensor) {
  uint16_t val = analog_getValueExt(sensor, 2);
  // If value bigger than 255, return 255
  if (val>0xff) return 0xff;
  return val;
}


uint8_t obstacle_getEvent(uint8_t reset) {
	// Diese Variable wird nur beim aller ersten Aufruf der Funktion hier zugewiesen weil static.
  static uint8_t event_last = EVENT_NONE;
  
  if (reset) {
    event_last = EVENT_NONE;
    return EVENT_NONE;
  }
  
  uint8_t event = EVENT_NONE;
  // Um ein bit nach rechts schieben. verliere auflösung (dividiert durch 2)
  uint8_t l = getObstSensorValue(ANALOG_FL)>>1;
  uint8_t r = getObstSensorValue(ANALOG_FR)>>1;
  
  uint8_t ll = getObstSensorValue(ANALOG_FLL)>>1;
  uint8_t rr = getObstSensorValue(ANALOG_FRR)>>1;
  // hier wird zahl im worst case doppelt so gross
  uint8_t cc = l + r;
  ll += l;
  rr += r;

  // Three values: ll (left-left), cc (center-center) and  rr (right-right)
  obstacle_val = max3(ll, cc, rr);
  
  if ((obstacle_val==cc) || (ll==rr)) {
    if (ll>rr) {
      // Obstacle slightly left
      obstacle_pos = -1;
    } else if (ll<rr) {
      // Obstacle slightly right
      obstacle_pos = +1;
    } else {
      // Obstacle in the middle
      obstacle_pos = 0;
    }
  } else if (ll>rr) {
     // Obstacle left
    obstacle_pos = -2;
  } else {
    // Obstacle right
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
    
	// nur falls event geändert hat, ansonsten mache nichts
  if (event!=event_last) {
    event_last = event;
    return event;
  }
  
  return EVENT_NONE;
}

/************************************************************************/
/* KEY                                                                  */
/************************************************************************/

uint8_t key_getEvent() {
  uint8_t c = key_get_char();
  if (c=='a') return EVENT_KEY1;
  if (c=='b') return EVENT_KEY2;
  if (c=='c') return EVENT_KEY3;
  return EVENT_NONE;
}

// kam ein tastendruck oder hat sich etwas mit der obstracle detection geändert
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
  
  
  // warten bis starttaste gedrück
  while (key_getEvent()==EVENT_NONE) {
    // wait...
  }
  
  delay(100);
  delay(100);
   
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
