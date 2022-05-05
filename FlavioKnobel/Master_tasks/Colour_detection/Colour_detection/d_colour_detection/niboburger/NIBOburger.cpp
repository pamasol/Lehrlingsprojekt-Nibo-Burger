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

/*! @file    NIBOburger.cpp
 *  @brief   Diese Datei enthält die Klassen zum NIBO burger Roboter
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-06-22
 */
#define __STDC_LIMIT_MACROS
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include "niboburger/iodefs.h"
#include "niboburger/led.h"
#include "niboburger/analog.h"
#include "niboburger/surface.h"
#include "niboburger/base.h"
#include "niboburger/odometry.h"
#include "niboburger/clock.h"
#include "niboburger/motpwm.h"
#include "niboburger/motpid.h"
#include "niboburger/key.h"
#include "niboburger/color.h"

#include "NIBOburger.h"



EnginePartClass::EnginePartClass(unsigned char _id) : id(_id) {
}

int EnginePartClass::getTicks() {
  if (id) {
    return odometry_getRight(0);
  } else {
    return odometry_getLeft(0);
  }
}

void EnginePartClass::resetTicks() {
  if (id) {
    odometry_getRight(1);
  } else {
    odometry_getLeft(1);
  }
}


/*--------------------------------------------------------*/

EngineClass::EngineClass() : left(0), right(1) {
}


void EngineClass::begin() {
  motpwm_init();
  motpid_init();
  odometry_init();
}


int EngineClass::getTargetDist() {
  return motpid_getTargetDist();
}


void EngineClass::setPWM(int left, int right) {
  motpid_release();
  motpwm_setRight(right);
  motpwm_setLeft(left);
}


void EngineClass::setSpeed(int left, int right) {
  motpid_setSpeed(left, right);
}


void EngineClass::setTargetAbs(int left, int right, unsigned int speed) {
  motpid_setTarget(left, right, speed);
}


void EngineClass::setTargetRel(int left, int right, unsigned int speed) {
  motpid_setTargetRel(left, right, speed);
}


void EngineClass::stopImmediate() {
  motpid_stop(1);
}


void EngineClass::stop() {
  motpid_stop(0);
}


/*---------------------------------------------------------------------------*/


SurfaceSensorClass::SurfaceSensorClass() {
}


void SurfaceSensorClass::begin() {
  surface_init();
  surface_readPersistent();
}


unsigned int SurfaceSensorClass::get(unsigned char id, unsigned char absolute) {
  return 0;
}


unsigned int SurfaceSensorClass::getL(unsigned char absolute) {
  return surface_get(SURFACE_L);
}


unsigned int SurfaceSensorClass::getC(unsigned char absolute) {
  return surface_get(SURFACE_C);
}


unsigned int SurfaceSensorClass::getR(unsigned char absolute) {
  return surface_get(SURFACE_R);
}

unsigned int SurfaceSensorClass::getCL(unsigned char absolute) {
  return surface_get(SURFACE_CL);
}

unsigned int SurfaceSensorClass::getCR(unsigned char absolute) {
  return surface_get(SURFACE_CR);
}

void SurfaceSensorClass::calibrateWhite() {
  surface_calibrateWhite();
}

void SurfaceSensorClass::calibrateBlack() {
  surface_calibrateBlack();
}


void SurfaceSensorClass::storeCalibration() {
  surface_writePersistent();
}


ColorRGB SurfaceSensorClass::getColorRGB() {
  return ColorRGB(surface_getColorRGB());
}

ColorHSV SurfaceSensorClass::getColorHSV() {
  return ColorHSV(surface_getColorHSV());
}


/*---------------------------------------------------------------------------*/

SensorClass::SensorClass(unsigned char _id) {
  id = _id;
}


void SensorClass::activate(unsigned char mode) {
  switch(mode) {
    case SENSOR_DISABLED: // digital bit disconnected
    case SENSOR_PULSE:    // toggle digital bit
    case SENSOR_LOW:      // digital bit output low (0 V)
    case SENSOR_HIGH:     // digital bit output high (5 V)
      break;
  }
}


void SensorClass::deactivate() {
  activate(SENSOR_DISABLED);
}


unsigned int SensorClass::get(unsigned char absolute) {
  int16_t val16;
  val16 = (int16_t)analog_getValueExt(id, absolute?ANALOG_PASSIVE:ANALOG_DIFFERENTIAL);
  return val16;
}


/*---------------------------------------------------------------------------*/


NIBOburgerClass::NIBOburgerClass() {
}


void NIBOburgerClass::begin() {
  IO_LEDS_DDR |= IO_LEDS_MASK;
  
  Engine.begin();
  SurfaceSensor.begin();
  _delay_ms(100);
}


unsigned int NIBOburgerClass::getVoltage() {
  return nibo_getMillivolt();
}


void NIBOburgerClass::checkVoltage() {
  nibo_checkMonitorVoltage();
}

void NIBOburgerClass::setMonitorVoltage(unsigned int millivolt) {
  nibo_setMonitorVoltage(millivolt);
}


void NIBOburgerClass::setLeds(bool led1, bool led2, bool led3, bool led4) {
  led_setall(led1, led2, led3, led4);
}

void NIBOburgerClass::setLed(int led, int value) {
  led_set(led, value);
}


int NIBOburgerClass::getLed(int led) {
  return led_get(led);
}

void NIBOburgerClass::waitAnalogUpdate() {
  analog_wait_update();
}

unsigned int NIBOburgerClass::getAnalog(unsigned char channel, unsigned char active) {
  return analog_getValueExt(channel, active);
}

void NIBOburgerClass::randomize(unsigned int seed) {
  srand(seed);
}

unsigned int NIBOburgerClass::getRandomSeed() {
  return analog_getRandomSeed();
}

void NIBOburgerClass::randomize() {
  srand(analog_getRandomSeed());
}

unsigned int NIBOburgerClass::getKeyState() {
  return key_get_state();
}

unsigned int NIBOburgerClass::getKeyChar() {
  return key_get_char();
}

int NIBOburgerClass::getRandomInt(int lo, int hi) {
  unsigned int n = hi-lo + 1;
  unsigned int k = RAND_MAX / n;
  k = k*n;
  unsigned int res;
  do {
    res = rand();
  } while (res>=k);
  return lo + (res%n);
}

/*---------------------------------------------------------------------------*/

ColorRGB::ColorRGB(unsigned char r, unsigned char g, unsigned char b) 
: value((((unsigned long)r)<<16) | (((unsigned long)g)<<8) | (((unsigned long)b)<<0)) {
}

ColorRGB::ColorRGB(unsigned long val)
: value(val) {
}

ColorRGB::~ColorRGB() {
}

unsigned int ColorRGB::diff(const ColorRGB & col2) const {
  return color_diff_rgb(this->value, col2.value);
}

unsigned int ColorRGB::diff(unsigned long val2) const {
  return color_diff_rgb(this->value, val2);
}

/*---------------------------------------------------------------------------*/

ColorHSV::ColorHSV(unsigned char h, unsigned char s, unsigned char v)
: value((((unsigned long)h)<<16) | (((unsigned long)s)<<8) | (((unsigned long)v)<<0)) {
}

ColorHSV::ColorHSV(unsigned long val)
: value(val) {  
}

ColorHSV::~ColorHSV() {
}

unsigned int ColorHSV::diff(const ColorHSV & col2, unsigned int hTol) const {
  return color_diff_hsv_bal(this->value, col2.value, hTol);
}

unsigned int ColorHSV::diff(unsigned long val2, unsigned int hTol) const {
  return color_diff_hsv_bal(this->value, val2, hTol);
}

/*---------------------------------------------------------------------------*/

NIBOburgerClass NIBOburger;
EngineClass Engine;
SurfaceSensorClass SurfaceSensor;

SensorClass SensorFL(ANALOG_FL);
SensorClass SensorFR(ANALOG_FR);
SensorClass SensorFLL(ANALOG_FLL);
SensorClass SensorFRR(ANALOG_FRR);
SensorClass SensorBL(ANALOG_BL);
SensorClass SensorBC(ANALOG_BC);
SensorClass SensorBR(ANALOG_BR);
