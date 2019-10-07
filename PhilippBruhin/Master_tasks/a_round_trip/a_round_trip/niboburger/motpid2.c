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

/*! @file    motpid2.c
 *  @brief   Neuer PID Regler
 *  @author  Nils Springob (nils.springob@nicai-systems.de)
 *  @date    2015-06-06
 */

#include <avr/interrupt.h>
#include <util/atomic.h>
#include "niboburger/iodefs.h"
#include "niboburger/base.h"
#include "niboburger/clock.h"
#include "niboburger/motpwm.h"
#include "niboburger/motpid2.h"

#ifndef TIMSK
# define PWM_TIMER_IMSK     TIMSK1
#else
# define PWM_TIMER_IMSK     TIMSK
#endif

#define PWM_TIMER_IMSK_OIE   _BV(TOIE1)

// Clock = 15 Mhz
// PS = 1
// freq = 15Mhz/1022 = 14.677 kHz
// call PID with 64 Hz:

#define MOTPID_CALL_CNT 229

/*! PID will be called with 64 Hz */
#define MOTPID_FREQ 64

#define MOTPID_KP 100
#define MOTPID_KI 0
//#define MOTPID_KI 20
#define MOTPID_KD 0

#define MOTPID_KPR 6
#define MOTPID_KIR 6
#define MOTPID_KDR 7

#define MOTPID_KPT 6
#define MOTPID_KIT 6
#define MOTPID_KDT 7

#define MAX_OUTPUT 1024
#define MAX_SPEED_SLOPE 500

#define IS_NEG(a) (((uint16_t)(a))&0x8000U)

static int16_t clip_add16(int16_t a, int16_t b) {
  int16_t s = a+b;
  if (IS_NEG(a^b)) {
    return s;
  }
  if (IS_NEG(a)) {
    if (IS_NEG(s)) {
      return s;
    }
    return 0x7fff;
  } else {
    if (IS_NEG(s)) {
      return 0x8000;
    }
    return s;
  }
}


#ifdef __cplusplus
extern "C" {
#endif



uint8_t motpid_cnt;

extern volatile int16_t odometry_left;
extern volatile int16_t odometry_right;
extern int16_t odometry_usr_left;
extern int16_t odometry_usr_right;

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

enum {
  MOTPID_MODE_OFF   = 0x00,  // No PID
  MOTPID_FLAG_SPEED = 0x01,  // Speed
  MOTPID_FLAG_POS   = 0x02,  // Position
  MOTPID_FLAG_XING  = 0x10   // left/right vs. trans/rot
};



// originale Basis
typedef struct {
  // input
  int16_t tspeed;      // target speed
  int16_t tpos;        // target position
  int16_t pos;         // actual position
  uint8_t mode;        // Modus                 (w/r)
  // internal
  int16_t calc_speed;  // calculated target speed 
  int16_t calc_pos;    // calculated target position
} pid_in_t;


// transponierte Basis
typedef struct {
  int16_t target;            // in
  int16_t goal;              // in
  int16_t value_act;         // in
  int16_t value_last;        // temp
} pid_temp_t;


typedef struct {
  int16_t setpoint;         // in
  int16_t input;            // in
  int16_t output;           // out
  
  // dyndata für Regler
  int16_t md;               //  differentiell        (-/rw)
  int16_t mi;               //  integral             (-/rw)
  
  // Parameter für Regler
  int8_t kp;                //                       (w/r)
  int8_t ki;                //                       (w/r)
  int8_t kd;                //                       (w/r)
  int16_t mi_max;
} pid_t;

typedef struct {
  int16_t value;      // 
  int16_t akku;          // 
} pid_out_t;



typedef struct {
  pid_in_t in;
  pid_temp_t temp;
  pid_t pid;
  pid_out_t out;
} motpid_t;


static motpid_t motpid[2];
#define motpid_l motpid[0]
#define motpid_r motpid[1]


void motpid_reset(uint8_t side) {
  motpid[side].pid.setpoint = 0;
  motpid[side].pid.input = 0;
  motpid[side].pid.output = 0;
  motpid[side].pid.md = 0;
  motpid[side].pid.mi = 0;
}


void motpid_change_mode(uint8_t new_mode, uint8_t side) {
  if (motpid[side].in.mode==new_mode) {
    return;
  }
  motpid_reset(side);
  motpid[side].in.mode = new_mode;
  if (new_mode&MOTPID_FLAG_XING) {
    if (side==0) {
      motpid[side].pid.kp = MOTPID_KPR;
      motpid[side].pid.ki = MOTPID_KIR;
      motpid[side].pid.kd = MOTPID_KDR;
    } else {
      motpid[side].pid.kp = MOTPID_KPT;
      motpid[side].pid.ki = MOTPID_KIT;
      motpid[side].pid.kd = MOTPID_KDT;
    }    
  } else {
    motpid[side].pid.kp = MOTPID_KP;
    motpid[side].pid.ki = (32L*MOTPID_KI)/MOTPID_FREQ;
    motpid[side].pid.kd = MOTPID_KD;
  }
}


static void motpid_calculate_input(motpid_t * data) {
  int16_t speed = data->in.calc_speed;

  // throttle speed when approaching target
  if (data->in.mode & MOTPID_FLAG_POS) {
    int16_t dist = data->in.tpos - data->in.pos;
    if ((dist>-40)&&(dist<40)) {
      speed = dist; // FIXME: find better solution....
    }
  }
  
  // trim to maximum speed slope
  if (data->in.tspeed > speed) {
    speed += MAX_SPEED_SLOPE/MOTPID_FREQ;
    if (speed>data->in.tspeed) {
      speed = data->in.tspeed;
    }
  } else if (data->in.tspeed < speed) {
    speed -= MAX_SPEED_SLOPE/MOTPID_FREQ;
    if (speed<data->in.tspeed) {
      speed = data->in.tspeed;
    }
  }
  
  data->in.calc_speed = speed;
  data->in.calc_pos += speed;  
  data->pid.input = data->in.calc_pos - (MOTPID_FREQ * data->in.pos);
}





#define INTERVAL(a,b,c) (((a)<(b))?(((b)<(c)?(b):(c))):(a))

static void motpid_calculate_pid(motpid_t * data) {
  // limit: Sollwert
  // value: Istwert
  // a0, a1: PID Speicher
  
  int16_t in = data->pid.input;
  int16_t temp = 0;
  temp += data->pid.kp * in;
  data->pid.mi = clip_add16(data->pid.mi, data->pid.ki * in);
  if (data->pid.mi > data->pid.mi_max) {
    data->pid.mi = data->pid.mi_max;
  } else if (data->pid.mi < -data->pid.mi_max) {
    data->pid.mi = -data->pid.mi_max;
  }
  temp += data->pid.mi/32;
  temp += data->pid.kd * (in - data->pid.md);
  data->pid.md = in;
  data->pid.output = temp;
}



// 100 Hz
static void motpid_work() {
  if (motpid[0].in.mode||motpid[1].in.mode) {
    motpid[0].in.pos = odometry_left;
    motpid[1].in.pos = odometry_right;
    
    if (motpid[0].in.mode) {
      motpid_calculate_input(&motpid[0]);
      motpid_calculate_pid(&motpid[0]);
    }
    
    if (motpid[1].in.mode) {
      motpid_calculate_input(&motpid[1]);
      motpid_calculate_pid(&motpid[1]);
    }

    if (motpid[0].in.mode) {
      motpwm_setLeft(motpid[0].out.value);
    }
    
    if (motpid[1].in.mode) {
      motpwm_setRight(motpid[1].out.value);
    }
    
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
  if (counter>=15) {
    counter-=15;
    clock_inc_irq(1, 22);
  }

  motpid_cnt++;
  if (motpid_cnt>=MOTPID_CALL_CNT) {
    motpid_work();
    motpid_cnt=0;
  }
  running = 0;
}


void motpid_init() {
  if (!(nibo_initialization&NIBO_MOTPWM_INITIALIZED)) {
    motpwm_init();
  }
  PWM_TIMER_IMSK |= PWM_TIMER_IMSK_OIE;
  motpid_reset(0);
  motpid_reset(1);
  motpid[0].in.mode = MOTPID_MODE_OFF;
  motpid[1].in.mode = MOTPID_MODE_OFF;
}






///////////////////////  PUBLIC  ///////////////////////

// void motpid_init()


void motpid_release() {
  motpid_change_mode(MOTPID_MODE_OFF, MOTOR_LEFT);
  motpid_change_mode(MOTPID_MODE_OFF, MOTOR_RIGHT);
}


void motpid_stop(uint8_t immediate) {
  if (immediate) {
    motpid_change_mode(MOTPID_MODE_OFF, MOTOR_LEFT);
    motpid_change_mode(MOTPID_MODE_OFF, MOTOR_RIGHT);
    motpwm_setRight(0);
    motpwm_setLeft(0);
  } else {
    motpid_setSpeed(0, 0);
  }
}


void motpid_setSpeed(int16_t left, int16_t right) {
    motpid_change_mode(MOTPID_FLAG_SPEED, MOTOR_LEFT);
    motpid_change_mode(MOTPID_FLAG_SPEED, MOTOR_RIGHT);
    motpid[MOTOR_LEFT].in.tspeed = left;
    motpid[MOTOR_RIGHT].in.tspeed = right;
    motpid[MOTOR_LEFT].in.calc_pos = odometry_left;
    motpid[MOTOR_RIGHT].in.calc_pos = odometry_right;
}


void motpid_setTarget(int16_t left, int16_t right, uint16_t speed) {
    motpid_change_mode(MOTPID_FLAG_POS, MOTOR_LEFT);
    motpid_change_mode(MOTPID_FLAG_POS, MOTOR_RIGHT);
    motpid[MOTOR_LEFT].in.tpos = left + odometry_usr_left;
    motpid[MOTOR_RIGHT].in.tpos = right + odometry_usr_right;
    motpid[MOTOR_LEFT].in.tspeed = speed;
    motpid[MOTOR_RIGHT].in.tspeed = speed;
    motpid[MOTOR_LEFT].in.calc_pos = odometry_left;
    motpid[MOTOR_RIGHT].in.calc_pos = odometry_right;
}


void motpid_setTargetRel(int16_t left, int16_t right, uint16_t speed) {
    motpid_change_mode(MOTPID_FLAG_POS, MOTOR_LEFT);
    motpid_change_mode(MOTPID_FLAG_POS, MOTOR_RIGHT);
    motpid[MOTOR_LEFT].in.tspeed = speed;
    motpid[MOTOR_RIGHT].in.tspeed = speed;
    motpid[MOTOR_LEFT].in.tpos = odometry_left + left;
    motpid[MOTOR_RIGHT].in.tpos = odometry_right + right;
    motpid[MOTOR_LEFT].in.calc_pos = odometry_left;
    motpid[MOTOR_RIGHT].in.calc_pos = odometry_right;
}


#ifdef __cplusplus
} // extern "C"
#endif

