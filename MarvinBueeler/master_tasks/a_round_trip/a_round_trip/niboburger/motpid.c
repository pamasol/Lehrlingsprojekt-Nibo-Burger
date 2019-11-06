/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

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

/*! @file    motpid.c
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-06-06
 */

#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include "niboburger/iodefs.h"
#include "niboburger/base.h"
#include "niboburger/clock.h"
#include "niboburger/motpwm.h"
#include "niboburger/motpid.h"
#include "niboburger/odometry.h"

#ifndef TIMSK
# define PWM_TIMER_IMSK     TIMSK1
#else
# define PWM_TIMER_IMSK     TIMSK
#endif

#define PWM_TIMER_IMSK_OIE   _BV(TOIE1)

// call PID with 100 Hz:
#define MOTPID_CALL_CNT ((uint8_t)((F_CPU/1022 + 50)/100))
// Example for 15 MHz:
// Clock = 15 Mhz, (PS = 1)
// freq = 15 Mhz / 1022 = 14.677 kHz
// ->  MOTPID_CALL_CNT = 147

// Example for 20 MHz:
// Clock = 20 Mhz, (PS = 1)
// freq = 20 Mhz / 1022 = 19.569 kHz
// ->  MOTPID_CALL_CNT = 196

#define CLOCK_CALL_CNT (F_CPU/1000000UL)
// 15 MHz: 15
// 20 MHz: 20


/*! PID will be called with 100 Hz */
#define MOTPID_FREQ 100

/*
#define MOTPID_KP 6
#define MOTPID_KI 6
#define MOTPID_KD 4
*/
#define MOTPID_KP 10
#define MOTPID_KI 10
#define MOTPID_KD 7

#ifdef __cplusplus
extern "C" {
#endif



uint8_t motpid_cnt;

extern volatile int8_t odometry_pid_left;
extern volatile int8_t odometry_pid_right;
extern volatile int16_t odometry_left;
extern volatile int16_t odometry_right;

enum {
  MOTPID_MODE_OFF = 0x00,
  MOTPID_MODE_SPEED = 0x03,
  MOTPID_MODE_POS = 0x04
};


int16_t motpid_left_speed;
int16_t motpid_right_speed;
int16_t motpid_left_speed2;
int16_t motpid_right_speed2;

int16_t motpid_position_speed=20;

int16_t motpid_left_value;
int16_t motpid_right_value;

int8_t motpid_left_cnt;
int8_t motpid_right_cnt;

uint8_t motpid_mode;

int16_t motpid_straight_dir;
uint8_t motpid_straight;
int8_t motpid_straight_corr_pwm;
int8_t motpid_straight_corr_pid;

typedef struct {
  int16_t a2;
  int16_t a1;
  int16_t o1;
}
motpid_dyndata_t;

static motpid_dyndata_t motpid_dyndata_left;
static motpid_dyndata_t motpid_dyndata_right;


void motpid_reset(void) {
  motpid_dyndata_left.a2 = 0;
  motpid_dyndata_left.a1 = 0;
  motpid_dyndata_left.o1 = 0;
  motpid_dyndata_right.a2 = 0;
  motpid_dyndata_right.a1 = 0;
  motpid_dyndata_right.o1 = 0;
}

// special function
void modpid_load_pwm(int16_t l, int16_t r) {
  motpid_dyndata_left.o1 = 16 * l;
  motpid_dyndata_right.o1 = 16 * r;
}


void motpid_change_mode(uint8_t new_mode) {
  if (motpid_mode==new_mode)
    return;
  motpid_reset();
  motpid_mode=new_mode;
}


static int16_t motpid_calculate_single(int16_t a2, int16_t a1, int16_t o1, int16_t setpoint, int16_t reading) {
  int16_t temp = o1;
  if (temp >= +16384) temp = +16384;
  if (temp <= -16384) temp = -16384;
  temp += MOTPID_KI * (setpoint - reading);
  temp += MOTPID_KD * (2*a1 - a2 - reading);
  temp += MOTPID_KP * (a1 - reading);
  return temp;
}


void motpid_left_timer_int(void) {
  motpid_left_speed = (3*motpid_left_speed + (MOTPID_FREQ/2)*odometry_pid_left)/4;
  motpid_left_speed2 = (3*motpid_left_speed2 + motpid_left_speed)/4;
  if (motpid_mode) {
    int16_t value = motpid_left_speed;
    int16_t setpoint = motpid_left_value;
    if (motpid_mode==MOTPID_MODE_POS) {
      setpoint -= odometry_left;
      if (setpoint>motpid_position_speed) setpoint=motpid_position_speed;
      if (setpoint<-motpid_position_speed) setpoint=-motpid_position_speed;
      setpoint += motpid_straight_corr_pid;
      if (setpoint==0) {
        motpid_dyndata_left.a2 = motpid_dyndata_left.a1 = motpid_dyndata_left.o1 = 0;
      }
    }

    motpid_dyndata_left.o1 = motpid_calculate_single(motpid_dyndata_left.a2,  motpid_dyndata_left.a1,
        motpid_dyndata_left.o1, setpoint,  value);
    motpid_dyndata_left.a2 = motpid_dyndata_left.a1;
    motpid_dyndata_left.a1 = value;

    value = motpid_dyndata_left.o1/16;
    value += motpid_straight_corr_pwm;
    
    if (value>0) value += 40;
    if (value<0) value -= 40;
    motpwm_setLeft(value);
  }
  odometry_pid_left = 0;
}


void motpid_right_timer_int(void) {
  motpid_right_speed = (3*motpid_right_speed + (MOTPID_FREQ/2)*odometry_pid_right)/4;
  motpid_right_speed2 = (3*motpid_right_speed2 + motpid_right_speed)/4;
  if (motpid_mode) {
    int16_t value = motpid_right_speed;
    int16_t setpoint = motpid_right_value;
    if (motpid_mode==MOTPID_MODE_POS) {
      setpoint -= odometry_right;
      if (setpoint>motpid_position_speed) setpoint=motpid_position_speed;
      if (setpoint<-motpid_position_speed) setpoint=-motpid_position_speed;
      setpoint -= motpid_straight_corr_pid;
      if (setpoint==0) {
        motpid_dyndata_right.a2 = motpid_dyndata_right.a1 = motpid_dyndata_right.o1 = 0;
      }
    }

    motpid_dyndata_right.o1 = motpid_calculate_single(motpid_dyndata_right.a2,  motpid_dyndata_right.a1,
        motpid_dyndata_right.o1, setpoint,  value);
    motpid_dyndata_right.a2 = motpid_dyndata_right.a1;
    motpid_dyndata_right.a1 = value;

    value = motpid_dyndata_right.o1/16;
    value -= motpid_straight_corr_pwm;
    
    if (value>0) value += 40;
    if (value<0) value -= 40;
    motpwm_setRight(value);
  }
  odometry_pid_right = 0;
}

void motpid_dir_int() {
  if (motpid_straight) {
    int16_t motpid_dir = (odometry_right-odometry_left);
    motpid_dir -= motpid_straight_dir;
    if (motpid_dir<-10) {
      motpid_dir = -10;
    } else if (motpid_dir>10) {
      motpid_dir = 10;
    }
    motpid_straight_corr_pwm = 5*motpid_dir;
    motpid_straight_corr_pid = ((motpid_dir>0)?1:((motpid_dir<0)?-1:0));
  } else {
    motpid_straight_corr_pwm = motpid_straight_corr_pid = 0;
  }
}



ISR(TIMER1_OVF_vect) {
  static uint8_t counter=0;
  static uint8_t running=0;
  counter++;
  if (running) {
    return;
  }
  running=1;
  if (counter >= CLOCK_CALL_CNT) {
    counter -= CLOCK_CALL_CNT;
    // called every 1.022 ms
    clock_inc_irq(1, 22);
#ifdef ARDUINO
    arduino_increment_time_isr(1, 22);
#endif
  }

  motpid_cnt++;
  if (motpid_cnt>=MOTPID_CALL_CNT) {
    motpid_dir_int();
    motpid_left_timer_int();
    //schedule_irq();
    motpid_right_timer_int();
    //schedule_irq();
    //comdata_signal();
    motpid_cnt=0;
  }
  running = 0;
}


void motpid_init() {
  if (!(nibo_initialization&NIBO_MOTPWM_INITIALIZED)) {
    motpwm_init();
  }
  PWM_TIMER_IMSK |= PWM_TIMER_IMSK_OIE;
  motpid_reset();
  motpid_mode = MOTPID_MODE_OFF;
}


void motpid_release() {
  motpid_change_mode(MOTPID_MODE_OFF);
}


void motpid_stop(uint8_t immediate) {
  if (immediate) {
    motpid_change_mode(MOTPID_MODE_OFF);
    motpwm_setRight(0);
    motpwm_setLeft(0);
  } else {
    motpid_setSpeed(0, 0);
  }
}


void motpid_setSpeed(int16_t left, int16_t right) {
    motpid_change_mode(MOTPID_MODE_SPEED);
    motpid_left_value = left;
    motpid_right_value = right;
}


void motpid_setTarget(int16_t left, int16_t right, uint16_t speed) {
    motpid_setTargetRel(left-odometry_left, right-odometry_right, speed);
}


void motpid_setTargetRel(int16_t left, int16_t right, uint16_t speed) {
    motpid_change_mode(MOTPID_MODE_POS);
    motpid_position_speed = speed;
    motpid_left_value  = odometry_left  + left;
    motpid_right_value = odometry_right + right;
    motpid_straight = (left==right);
    motpid_straight_dir = (odometry_right-odometry_left);
}


uint16_t motpid_getTargetDist() {
  if (motpid_mode&MOTPID_MODE_POS) {
    return abs(motpid_left_value - odometry_getLeft(0)) + abs(motpid_right_value - odometry_getRight(0));
  }
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

