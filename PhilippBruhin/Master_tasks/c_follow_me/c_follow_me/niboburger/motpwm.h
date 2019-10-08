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

/*! @file    motpwm.h
 *  @brief   Funktionen zur Erzeugung der PWM-Signale für die Motoren.
 * 
 * Mit dem PWM Signal wird die Leistung der Motoren und damit das Drehmoment gesteuert.
 * In Abhängigkeit von der Getriebereibung und der Rollreibung des jeweiligen Rades 
 * ergibt sich eine Drehgeschwindigkeit. Diese ist jedoch ungeregelt und wird von
 * vielen Umweltbedingungen beeinflusst. Für eine geregelte Geradeausfahrt muss
 * die Datei <niboburger/motpwm.h> verwendet werden (auch diese verwendet intern die PWM
 * Steuerung). Die PWM Ansteuerung ist jedoch sehr schnell und bietet insbesondere bei
 * Linienverfolgung und Hindernisvermeidung einige Vorteile.
 * 
 */

#ifndef _NIBOBURGER_MOTPWM_H_
#define _NIBOBURGER_MOTPWM_H_

#ifdef __cplusplus
extern "C" {
#endif



/*!
 * Initialiserung für PWM.
 */
void motpwm_init();


/*!
 * PWM Signal abschalten - beide Motoren stoppen.
 */
void motpwm_stop();


/*!
 * PWM Wert für linken Motor setzen.
 * @param value muss in Interval [-1023 ... +1023] (entspricht -100% .. +100% Leistung) liegen.
 */
void motpwm_setLeft(int16_t value);


/*!
 * PWM Wert für linken Motor holen.
 * @return liegt im Interval [-1023 ... +1023] (entspricht -100% .. +100% Leistung)
 */
int16_t motpwm_getLeft();


/*!
 * PWM Wert für rechten Motor setzen.
 * @param value muss in Interval [-1023 ... +1023] (entspricht -100% .. +100% Leistung) liegen.
 */
void motpwm_setRight(int16_t value);


/*!
 * PWM Wert für rechten Motor holen.
 * @return liegt im Interval [-1023 ... +1023] (entspricht -100% .. +100% Leistung)
 */
int16_t motpwm_getRight();


#ifdef __cplusplus
} // extern "C"
#endif


#endif // _NIBOBURGER_MOTPWM_H_
