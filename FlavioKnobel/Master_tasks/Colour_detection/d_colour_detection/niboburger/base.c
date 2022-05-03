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

/*! @file   base.c
 *  @brief  Verschiedene Funktionen für den Roboter
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date   2015-03-31
 */

#include "niboburger/base.h"
#include "niboburger/analog.h"
#include "niboburger/iodefs.h"
#include "niboburger/led.h"
#include "niboburger/motpwm.h"

#include <util/delay.h>

#if defined __AVR_ATmega1284P__ \
         || __AVR_ATmega644__ || __AVR_ATmega644A__ || __AVR_ATmega644P__ || __AVR_ATmega644PA__ \
         || __AVR_ATmega324__ || __AVR_ATmega324A__ || __AVR_ATmega324P__ || __AVR_ATmega324PA__ \
         || __AVR_ATmega164__ || __AVR_ATmega164A__ || __AVR_ATmega164P__
#define ADC_BANDGAP_CHANNEL_VOLTAGE 1.1
//#pragma message ( "ADC_BANDGAP 1.1 V" )
#else
#define ADC_BANDGAP_CHANNEL_VOLTAGE 1.22
//#pragma message ( "ADC_BANDGAP 1.22 V" )
#endif

#define DEFAULT_MIN_VOLTAGE (4.2-0.5)

#ifdef __cplusplus
extern "C" {
#endif


uint8_t nibo_initialization;
uint16_t nibo_monitor_voltage = (ADC_BANDGAP_CHANNEL_VOLTAGE*1024)/DEFAULT_MIN_VOLTAGE;

uint16_t nibo_getMillivolt() {
  uint16_t voltage = analog_getValue(ANALOG_VOLT);
  voltage = ((uint16_t)(50*ADC_BANDGAP_CHANNEL_VOLTAGE*1024))/voltage;
  voltage *= 20;
  return voltage;
}


static void delay(uint8_t count) {
  while (count--) {
    _delay_ms(100);
  }
}

static void blink_all(uint8_t on, uint8_t off) {
  led_set(LED_1, 1);
  led_set(LED_2, 1);
  led_set(LED_3, 1);
  led_set(LED_4, 1);
  delay(on);
  led_set(LED_1, 0);
  led_set(LED_2, 0);
  led_set(LED_3, 0);
  led_set(LED_4, 0);
  delay(off);
}

void nibo_setMonitorVoltage(uint16_t millivolt) {
  uint32_t val = (ADC_BANDGAP_CHANNEL_VOLTAGE*1024)*1000UL;
  val /= millivolt;
  nibo_monitor_voltage = val;
}


static void do_SOS() {
  activate_output_group(IO_LEDS);  // LED bits als Output
  motpwm_stop();
  deactivate_output_group(IO_MOTOR);
  disable_interrupts();

  while (1) {
    delay(5);
    blink_all(1,1); // S
    blink_all(1,1);
    blink_all(1,1);
    delay(2);
    blink_all(3,1); // O
    blink_all(3,1);
    blink_all(3,1);
    delay(2);
    blink_all(1,1); // S
    blink_all(1,1);
    blink_all(1,3);
    delay(10);
  }
}

void nibo_checkMonitorVoltage() {
  uint16_t voltage = analog_getValue(ANALOG_VOLT);
  if (voltage<nibo_monitor_voltage) {
    return;
  }
  do_SOS();
}

// obsolete
void nibo_assertMinVoltage(uint16_t value) {
  uint16_t voltage = analog_getValue(ANALOG_VOLT);
  if (voltage>value) {
    do_SOS();
  }
}


#ifdef __cplusplus
} // extern "C"
#endif
