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

/*! @file    base.h
 *  @brief   \~english Supply voltage functions and internal definitions for the NIBO burger Library
 *           \~german Funktionen für die Versorgungsspannung und interne Definitionen der NIBO burger Library
 * 
 * Die Versorgungsspannung des ATmega16A ist beim NIBO burger ca. 0.5 V unter der Spannung der Akkus.
 * Eine Versorgungsspannung von 4300 Millivolt entspricht einer Akkuspannung von 4,8 Volt
 */

#ifndef _NIBOBURGER_BASE_H_
#define _NIBOBURGER_BASE_H_


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \internal
 * \~english Initialization state values for the library modules
 * \~german Werte für die Initialisierungszustände der Module
 */
enum {
  NIBO_ANALOG_INITIALIZED = 0x01,
  NIBO_MOTPWM_INITIALIZED = 0x02,
  NIBO_I2C_INITIALIZED    = 0x04,
};

/*!
 * \internal
 * \~english Initialization state of NIBO burger Library modules
 * \~german Initialisierungszustand der NIBO burger Library Module
 */
extern uint8_t nibo_initialization;


/*!
 * @obsolete
 * \~english generate value for nibo_assertMinVoltage() function
 * @return value for nibo_assertMinVoltage() function
 * \deprecated Please use nibo_checkMonitorVoltage()!
 * \~german generiert den Wert für die nibo_assertMinVoltage() funktion
 * @return Wert für die nibo_assertMinVoltage() funktion
 * @deprecated Bitte nibo_checkMonitorVoltage() verwenden!
 */
#define NIBO_ASSERT_VOLTAGE(x) ((uint16_t)(1248/x))


/*!
 * \~english return supply voltage, requiers call to function analog_init()
 * @return accumulator voltage in millivolt
 * \~german Versorgungsspannung zurückgeben, erfordert den Aufruf der Funktion analog_init()
 * @return Versorgungsspannung in Millivolt
 */
uint16_t nibo_getMillivolt();


/*!
 * \~english check supply voltage, blink SOS signal and terminate if supply voltage is below.
 * @param value result from NIBO_ASSERT_VOLTAGE() macro
 * \deprecated Please use nibo_checkMonitorVoltage()!
 * \~german Versorgungsspannung überprüfen, SOS Blinken + Abbruch wenn die Versorgungsspannung niedriger ist. 
 * @param value : Ergebnis des NIBO_ASSERT_VOLTAGE() Makros
 * @deprecated Bitte nibo_checkMonitorVoltage() verwenden!
 * @code
nibo_assertMinVoltage(NIBO_ASSERT_VOLTAGE(3.7));
@endcode
 */
void nibo_assertMinVoltage(uint16_t value);


/*!
 * \~english Set monitor voltage (in millivolt).
 * \~german Die zu überprüfende Spannung (in Millivolt) festlegen
 * 
 * @code
void setup() {
  // Spannungsüberwachung auf 3.8 Volt (entspricht Akkuspannung ca. 4.3 Volt)
  nibo_setMonitorVoltage(3800);
  
  // ...
  
}
@endcode
 */
void nibo_setMonitorVoltage(uint16_t millivolt);


/*!
 * \~english check supply voltage, blink SOS signal (--- ... ---) and terminate if supply voltage is below.
 * \~german Versorgungsspannung überprüfen. Falls die Spannung niedriger ist als die mit nibo_setMonitorVoltage() definierte wird das
 * Programm abgebrochen und die LEDs zeigen das SOS Signal (--- ... ---) an.
 * 
 * @code
void loop() {
  // Prüfen ob die Versorgungsspannung noch OK ist:
  nibo_checkMonitorVoltage();
  
  // ...
  
}
@endcode
 */
void nibo_checkMonitorVoltage();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _NIBOBURGER_BASE_H_

