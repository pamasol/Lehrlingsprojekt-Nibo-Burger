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

/*! @file    robomain.h
 *  @brief   Ersetzt die main()-Funktion durch die beiden Funktionen setup() und loop(), wie in Processing...
 *  zusätzlich werden alle NIBO burger-Includes eingebunden! Die Datei darf nur von der Haupt-C-Datei eingebunden werden,
 *  alle anderen Dateien sollten die Datei <niboburger/robo.h> verwenden.
 * 
 * <b>Grundgerüst:</b>
 * @code
#include <niboburger/robomain.h>

void setup() {
  // diese Funktion wird einmalig ausgeführt, hier kann der Roboter initialisiert werden
  
  // ...
  
}

void loop() {
  // diese Funktion wird immer wieder automatisch ausgeführt
  
  // ...
  
}
@endcode
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2014-10-31
 */

#ifndef _NIBOBURGER_ROBOMAIN_H_
#define _NIBOBURGER_ROBOMAIN_H_

#include "robo.h"

/*!
 * Setup Funktion, wird einmalig am Anfang des Programms aufgerufen und muss selbst implementiert werden
 */
void setup();

/*!
 * Loop Funktion, wird immer wieder aufgerufen und muss selbst implementiert werden
 */
void loop();

/*!
 * Main Funktion eines C/C++ Programms, wird bei Processing (und Arduino) durch setup() und loop() ersetzt.
 */
int main () {
  sei();
  setup();
  while (1) {
    loop();
  }
}

#endif
