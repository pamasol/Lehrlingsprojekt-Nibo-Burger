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

/*!
 * @file    surface.h
 * @brief   Zugriff auf die aufbereiteten Daten des Boden- bzw. Farbsensors
 * 
 * Mit den Funktionen aus dieser Datei kann der Boden- bzw. Farbsensors kalibriert werden 
 * und es kann auf die kalibrierten Werte zugegriffen werden.
 * Die Werte @ref SURFACE_CL und @ref SURFACE_CR sind insbesondere für die Implementation 
 * einer Linienfolge interessant, da sie das Licht vom mittleren Sensorbrick mit dem Sensor
 * vom linken bzw. rechten Sensorbrick messen.
 */

#ifndef _NIBOBURGER_SURFACE_H_
#define _NIBOBURGER_SURFACE_H_

#include "niboburger/analog.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  Symbolische Namen für den Parameter idx der Funktionen
 *  surface_get() und surface_getAbs()
 */
enum {
  SURFACE_L  = ANALOG_BL,   //!< Bodensensor Links
  SURFACE_C  = ANALOG_BC,   //!< Bodensensor Mitte
  SURFACE_R  = ANALOG_BR,   //!< Bodensensor Rechts
  SURFACE_CL = ANALOG_BCL,  //!< Bodensensor Links, Licht von Mitte Sensor
  SURFACE_CR = ANALOG_BCR,  //!< Bodensensor Rechts, Licht von Mitte Sensor  
};


/*!
 * Initialisierung des Bodensensors.
 * Ruft die Funktionen analog_init() und line_readPersistent() automatisch auf.
 * Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 */
void surface_init();


/*!
 * Speichert die Kalibrierungsdaten im EEPROM des ATmega16
 */
void surface_writePersistent();


/*!
 * Laden der Kalibrierungsdaten aus dem EEPROM des ATmega16
 */
void surface_readPersistent();


/*!
 * Liefert den normalisierten Wert des Bodensensors mit dem index @param idx .
 * Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 * Der Parameter @param idx kann die Werte @ref SURFACE_L, @ref SURFACE_C, @ref SURFACE_R, @ref SURFACE_CL oder @ref SURFACE_CR annehmen.
 * @return Bei korrekt kalibrietem Sensor ein Wert aus dem Interval [0..1023]
 */
uint16_t surface_get(uint8_t idx);


/*!
 * 24 Bit RGB-Wert aus den Werten der kalibrierten Bodensensoren generieren
 */
uint32_t surface_getColorRGB();


/*!
 * 24 Bit HSV-Wert aus den Werten der kalibrierten Bodensensoren generieren
 */
uint32_t surface_getColorHSV();


/*!
 * Liefert den Wert des Bodensensors mit dem index @param idx bei ausgeschalteter LED.
 * Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 * Der Parameter @param idx kann die Werte @ref SURFACE_L, @ref SURFACE_C, @ref SURFACE_R annehmen.
 */
uint16_t surface_getAbs(uint8_t idx);


/*!
 * Kalibrierung auf weisse Oberfläche für normalisiserten Wert 1024.
 * Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 */
void surface_calibrateWhite();


/*!
 * Kalibrierung auf schwarze Oberfläche für normalisiserten Wert 0.
 * Die Interrupts müssen aktiviert sein, damit die Sensorwerte gemessen werden!
 * Die Funktion darf erst nach surface_calibrateWhite() aufgerufen werden.
 */
void surface_calibrateBlack();

#ifdef __cplusplus
} // extern "C"
#endif



#endif // _NIBOBURGER_SURFACE_H_
