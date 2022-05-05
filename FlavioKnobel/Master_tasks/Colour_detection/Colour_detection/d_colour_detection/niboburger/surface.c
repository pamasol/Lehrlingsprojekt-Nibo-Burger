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

/*! @file    surface.c
 *  @brief   Quellcode
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-06-06
 */

#include "niboburger/iodefs.h"
#include "niboburger/surface.h"
#include "niboburger/color.h"
#include "niboburger/base.h"
#include "niboburger/delay.h"
#include "niboburger/storage_nrl.h"

#include <avr/eeprom.h>


// for storage version=1  size = 2 * 5 * sizeof(uint16_t)
//#define STORAGE_NRL_SURFACE STORAGE_ENTRY(STORAGE_NRL_ID, STORAGE_NRL_SURFACE_ID, 1, 2*5*2)

#define RANGE 0x400
#define FACTOR 0x100
#define CALSAMPLES 0x20

#define GAIN_FACT ((uint32_t)RANGE * (uint32_t)FACTOR * (uint32_t)CALSAMPLES)
//      0x800000L =          0x400 *           0x100  *           0x20

#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

#define max3(a,b,c) max(max(a,b),c)
#define min3(a,b,c) min(min(a,b),c) 

#ifdef __cplusplus
extern "C" {
#endif

static uint16_t cal_gain[5];
static uint16_t cal_offset[5];

typedef struct { 
  uint16_t black[5]; // sum of CALSAMPLES samples
  uint16_t white[5]; // sum of CALSAMPLES samples
} line_cal_t;

line_cal_t line_cal;


static uint8_t index2sensor(uint8_t idx) {
  return (idx<3)?(idx+SURFACE_L):(idx+SURFACE_CL-3);
}

static uint8_t sensor2index(uint8_t sensor) {  
  return (sensor<=SURFACE_R)?(sensor-SURFACE_L):(sensor+3-SURFACE_CL);
}

void surface_init() {
  if (!(nibo_initialization & NIBO_ANALOG_INITIALIZED)) {
    analog_init();
  }
  surface_readPersistent();
}


static uint16_t get_differential(uint8_t idx) {
  int16_t res = (int16_t)analog_getValueExt(idx,1)-(int16_t)analog_getValueExt(idx,0);
  return (res>0)?res:0;
}


static uint16_t get_absolute0(uint8_t idx) {
  int16_t res = (int16_t)analog_getValue(idx);
  return (res>0)?res:0;
}


static void do_calculateCalibration(uint8_t idx) {
  // 0x800000L = 0x400 * 0x100 * 0x20
  // GAIN_FACT = range   factor calib
  cal_gain[idx] = GAIN_FACT / (line_cal.white[idx]-line_cal.black[idx]);
  cal_offset[idx] = line_cal.black[idx] / CALSAMPLES;
}


static void surface_calculateCalibration() {
  do_calculateCalibration(0);
  do_calculateCalibration(1);
  do_calculateCalibration(2);
  do_calculateCalibration(3);
  do_calculateCalibration(4);
}


void surface_writePersistent() {
  storage_write(STORAGE_NRL_SURFACE, &line_cal);
}


void surface_readPersistent() {
  if (storage_read(STORAGE_NRL_SURFACE, &line_cal)==0) {
    line_cal.white[0] = CALSAMPLES*0x200;
    line_cal.white[1] = CALSAMPLES*0x300;
    line_cal.white[2] = CALSAMPLES*0x200;
    line_cal.white[3] = CALSAMPLES*0x50;
    line_cal.white[4] = CALSAMPLES*0x50;
    line_cal.black[0] = CALSAMPLES*4;
    line_cal.black[1] = CALSAMPLES*4;
    line_cal.black[2] = CALSAMPLES*4;
    line_cal.black[3] = CALSAMPLES*4;
    line_cal.black[4] = CALSAMPLES*4;
  }
  surface_calculateCalibration();
}

/*
 * Differenz von LED an und aus mit Schwarzoffset
 */
uint16_t surface_get(uint8_t idx) {
  if ((idx<=SURFACE_R) || (idx==SURFACE_CL) || (idx==SURFACE_CR)) {
    uint16_t val16 = get_differential(idx);
    idx = sensor2index(idx);
    val16 -= (val16>cal_offset[idx])?cal_offset[idx]:val16;
    uint32_t val = val16;
    val *= cal_gain[idx];
    val /= FACTOR;
    if (val>INT16_MAX) {
      return INT16_MAX;
    }
    return val;
  }
  return 0;
} 


uint32_t surface_getColorRGB() {
  uint8_t r = min(255, surface_get(SURFACE_R)>>2);
  uint8_t g = min(255, surface_get(SURFACE_C)>>2);
  uint8_t b = min(255, surface_get(SURFACE_L)>>2);
  return color_convert_RGB2rgb(r, g, b);
}


uint32_t surface_getColorHSV() {
  uint8_t r = min(255, surface_get(SURFACE_R)>>2);
  uint8_t g = min(255, surface_get(SURFACE_C)>>2);
  uint8_t b = min(255, surface_get(SURFACE_L)>>2);
  return color_convert_RGB2hsv(r, g, b);
}



/*
 * Wert bei LED aus ohne Schwarzoffset
 */
uint16_t surface_getAbs(uint8_t idx) {
  if ((idx<=SURFACE_R) || (idx==SURFACE_CL) || (idx==SURFACE_CR)) {
    uint16_t val16 = get_absolute0(idx);
    idx = sensor2index(idx);
    uint32_t val = val16;
    val *= cal_gain[idx];
    val /= FACTOR;
    if (val>INT16_MAX) {
      return INT16_MAX;
    }
    return val;
  }
  return 0;
}

void surface_calibrateBlack() {
  uint8_t i;
  line_cal.black[0] = 0;
  line_cal.black[1] = 0;
  line_cal.black[2] = 0;
  line_cal.black[3] = 0;
  line_cal.black[4] = 0;
  for (i=0; i<CALSAMPLES; ++i) {
    analog_wait_update();
    line_cal.black[0] += get_differential(SURFACE_L);
    line_cal.black[1] += get_differential(SURFACE_C);
    line_cal.black[2] += get_differential(SURFACE_R);
    line_cal.black[3] += get_differential(SURFACE_CL);
    line_cal.black[4] += get_differential(SURFACE_CR);
  }
  surface_calculateCalibration();
}


void surface_calibrateWhite() {
  uint8_t i;
  line_cal.white[0] = 0;
  line_cal.white[1] = 0;
  line_cal.white[2] = 0;
  line_cal.white[3] = 0;
  line_cal.white[4] = 0;
  for (i=0; i<CALSAMPLES; ++i) {
    analog_wait_update();
    line_cal.white[0] += get_differential(SURFACE_L);
    line_cal.white[1] += get_differential(SURFACE_C);
    line_cal.white[2] += get_differential(SURFACE_R);
    line_cal.white[3] += get_differential(SURFACE_CL);
    line_cal.white[4] += get_differential(SURFACE_CR);
  }
  surface_calculateCalibration();
}

#ifdef __cplusplus
} // extern "C"
#endif

