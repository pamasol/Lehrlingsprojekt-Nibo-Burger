/*  BSD-License:

Copyright (c) 2015 by Nils Springob, nicai-systems, Germany

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
 *  @file    key.h
 *  \~german
 *  @brief   Routinen zur Abfrage der Tasten 1 - 3
 * 
 *  Es kann sowohl der aktuelle Zustand der Tasten mit der Funktion key_get_state(), als auch die Events mit der Funktion key_get_char() abgefragt werden.
 *  \~english
 *  @brief   functions for keypad
 *  \~
 */

#ifndef _NIBOBURGER_KEY_H_
#define _NIBOBURGER_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Symbolische Namen für den Rückgabewert der Funktion key_get_state()
 */
enum {
  KEY_STATE_NONE    = 0x00, //!< =0: keine Taste ist gedrückt
  KEY_STATE_1       = 0x01, //!< Taste 1 ist gedrückt
  KEY_STATE_2       = 0x02, //!< Taste 2 ist gedrückt
  KEY_STATE_3       = 0x04, //!< Taste 3 ist gedrückt
  KEY_STATE_INVALID = 0x80  //!< Taste ungültig
};

/*!
 * Symbolische Namen für die Events der Funktion key_get_char()
 * alternativ können auch die ASCII-Zeichen a, b, c, A, B, C und 
 * der Wert Null verwendet werden
 */
enum {
  KEY_EVENT_NONE      = '\0', //!< =0: kein Event / keine Änderung
  
  KEY_EVENT_1_DOWN    = 'A', //!< ='A': Taste 1 runtergedrückt
  KEY_EVENT_2_DOWN    = 'B', //!< ='B': Taste 2 runtergedrückt
  KEY_EVENT_3_DOWN    = 'C', //!< ='C': Taste 3 runtergedrückt
  
  KEY_EVENT_1_UP      = 'a', //!< ='a': Taste 1 losgelassen
  KEY_EVENT_2_UP      = 'b', //!< ='b': Taste 2 losgelassen
  KEY_EVENT_3_UP      = 'c', //!< ='c': Taste 3 losgelassen
};


/*!
 * \~german
 * aktuellen Zustand der Tasten liefern, die gedrückten Tasten werden ODER-verknüpft. Wenn 
 * der aktuelle Wert ungültig ist liefert die Funktion den Wert KEY_INVALID zurück, ist keine
 * Taste gedrück, liefert die Funktion 0 zurück.
 * \~english
 * current state of keypad, pressed keys are combined by binary OR. If curent value is invalid,
 * the function returns KEY_INVALID, if no key is pressed the function returns 0.
 */
uint8_t key_get_state();


/*!
 * liefert das nächste Key Event zurück. Key-Down-Events sind die Großbuchstaben A, B und C,
 * Key-Up-Events sind die Kleinbuchstaben a, b und c. Ist kein Key-Event vorhanden liefert die Funktion eine 0 zurück.
 * 
 * Ein einfacher Tastendruck (Drücken und Loslassen) auf den Button 1 liefert  somit nacheinander ein grosses 'A' und 
 * ein kleines 'a'. Davor, danach und dazwischen wird der Wert 0 zurückgeliefert. 
 * Erklärung:
 * Nachdem das grosse 'A' zurückgegeben wurde, liefert die Funktion den Wert 0 (kein Event) zurück solange der Taster
 * weiterhin gedrückt wird. Wenn der Taster losgelassen wird liefert die Funktion ein einzelnes kleines 'a' zurück und
 * danach wieder den Wert 0.
 * 
 * Beispiel: 0, 0, 0, 0, 0, 'A', 0, 0, 0, 0, 'a', 0, 0, 0, 0 
 * 
 */
char key_get_char();


#ifdef __cplusplus
} // extern "C"
#endif



#endif // _NIBOBURGER_KEY_H_
