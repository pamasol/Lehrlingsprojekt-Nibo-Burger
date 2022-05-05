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

/*! @file    clock.h
 *  @brief   Zeitzählung. Die Zeit steht in Sekunden und Millisekunden seit dem Start zur Verfügung.
 *
 *  Die Funktion clock_inc_irq() muss regelmäßig von einem Timer Interrupt aufgerufen
 *  werden, bei Verwendung des PID Reglers (motpid.h) geschieht dies automatisch alle
 *  1.022 ms! 
 */

#ifndef _NIBOBURGER_CLOCK_H_
#define _NIBOBURGER_CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Funktion zum Aufruf durch den IRQ. Wird bei Verwendung des PID Reglers
 * automatisch alle 1.022 ms aufgerufen. Falls der PID Regler nicht verwendet wird, muss die
 * Funktion regelmäßig von einem eigenen Timer-Interrupt aus aufgerufen werden.
 */
void clock_inc_irq(uint8_t ms, uint16_t us);


/*!
 * Wartet die angegebene Zeit (maximal 65 Sekunden (65535 ms)), die Zeit wird auch bei Ausführung
 * von Interrupt Funktionen eingehalten (anders als bei der delay() Funktionen)!
 */
void clock_sleep(uint16_t ms);


/*!
 * Zählt bis 1000 (1s) in 1ms Schritten
 */
extern volatile uint16_t clock_ms;


/*!
 * Zählt die Sekunden seit dem Systemstart
 */
extern volatile uint32_t clock_sec;


/*!
 * Wird jede Sekunde aufgerufen (Achtung: Funktion wird aus dem IRQ Kontext aufgerufen!)
 * Als 'weak-linking' Funktion kann sie einfach durch eine eigene Funktion void clock_sec_irq_hook() {} ersetzt 
 * werden.
 */
void clock_sec_irq_hook();


/*!
 * Wird jede hunderstel Sekunde aufgerufen (Achtung: Funktion wird aus dem IRQ Kontext aufgerufen!).
 * Als 'weak-linking' Funktion kann sie einfach durch eine eigene Funktion void clock_hundredth_irq_hook() {} ersetzt 
 * werden.
 */
void clock_hundredth_irq_hook();


/*!
 * Timer-Interval festlegen, auf den Ablauf des Timers kann man mit der Funktion clock_wait_timer() warten.
 * @param ms Interval in Millisekunden
 */
void clock_set_timer(uint16_t ms);


/*!
 * Die Funktion wartet auf den nächsten Ablauf des Timer-Intervals, das durch den Aufruf der Funktion clock_set_timer() festgelegt wurde.
 * Die CPU wird für die Wartezeit in den Stromspar-Modus geschickt...
 * @code
   // Ziel: Die Loop-Funktion soll alle 20 ms aufgeführt werden!
  
   void setup()  {
     clock_set_timer(20);
   }
   
   void loop() {
     // auf das nächste Interval warten:
     clock_wait_timer();
     
     // hierhin kommt man alle 20 ms:
     ...
     
   }
 * @endcode
 */
void clock_wait_timer();


/*!
 * Liefert die Zeit in Millisekunden zurück, die die Funktion clock_wait_timer() zu diesem Zeitpunkt warten würde.
 */
uint16_t clock_get_timer();


/*!
 * Liefert die Zeit in Millisekunden seit dem Systemstart zurück.
 * (32 Bit -> Überlauf erst nach 49 Tagen...)
 */
uint32_t clock_get_systime_ms();


/*!
 * Liefert die Zeit in Millisekunden seit dem letzten Aufruf der Funktion zurück (maximal ca. 65 Sekunden!).
 */
uint16_t clock_elapsed_ms();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _NIBOBURGER_CLOCK_H_
