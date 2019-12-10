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

/*! @file    odometry.h
 *  @brief   Funktionen zur Odometriemessung
 *  Mit den Odometriesensoren wird die Drehung der Räder gemessen. Daraus läßt sich der gefahrene Weg berechnen. Das Wort Odometrie kommt aus dem Griechischem und bedeutet Wegmessung.
 *  Die Drehung wird durch Lichtschranken und die Löcher in den roten Zahnrädern gemessen. Jedes Loch löst genau einen Zählimpuls aus, eine komplette Radumdrehung löst 100 (bzw. 20) 
 *  Impulse aus. Die Räder haben einen Durchmesser von 45 mm, das entspricht einem Umfang von ca. 141 mm. Pro Tick bewegt sich der Roboter somit 1,41 mm (bzw 7,05 mm) vorwärts.
 *  Die maximale Geschwindigkeit der Motoren im Leerlauf liegt bei ungefähr 100 Ticks/Sekunde - das sind ca. 14 cm / Sekunde (bzw. 70 cm / Sekunde).
 * 
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-03-31
 */

#ifndef _NIBOBURGER_ODOMETRY_H_
#define _NIBOBURGER_ODOMETRY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Initialisierung der Odometriemessung.
 * Die Lichtschranken lösen automatisch einen Interrupt aus, in dem die Zähler
 * aktualisisert werden. Die aktuelle Motorrichtung bestimmt ob die Zähler hoch
 * oder runter gezählt werden.
 * Die Interrupts müssen aktiviert sein!
 */
void odometry_init();


/*!
 * Setzt beide Zähler auf 0 zurück
 */
void odometry_reset();


/*!
 * Setzt den linken Zähler auf 0 zurück
 */
void odometry_resetLeft();


/*!
 * Setzt den rechten Zähler auf 0 zurück
 */
void odometry_resetRight();


/*!
 * Liefert den Wert des linken Zählers zurück. Je nach Motorrichtung wird der
 * Zähler hoch oder runter gezählt.
 * Wenn @param reset = 0 ist wird der Zählerstand beibehalten, ansonsten auf
 * 0 zurückgesetzt.
 */
int16_t odometry_getLeft(uint8_t reset);


/*!
 * Liefert den Wert des rechten Zählers zurück. Je nach Motorrichtung wird der
 * Zähler hoch oder runter gezählt.
 * Wenn @param reset = 0 ist wird der Zählerstand beibehalten, ansonsten auf
 * 0 zurückgesetzt.
 */
int16_t odometry_getRight(uint8_t reset);



#ifdef __cplusplus
} // extern "C"
#endif

#endif // _NIBOBURGER_ODOMETRY_H_
