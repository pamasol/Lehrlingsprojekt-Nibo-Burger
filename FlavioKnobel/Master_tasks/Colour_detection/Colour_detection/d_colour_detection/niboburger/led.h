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
 *  @file    led.h
 *  \~german
 *  @brief   Routinen zur Ansteuerung der LEDs
 *  \~english
 *  @brief   functions for LED control
 *  \~
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-03-31
 */

#ifndef _NIBOBURGER_LED_H_
#define _NIBOBURGER_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

  
/*!
 *  Symbolische Namen für die LEDs zur Verwendung in den Funktionen led_set() und  led_get().
 */
enum {
  LED_1 = IO_LEDS_BIT_1, //!< =1: LED 1 (links, rot)
  LED_2 = IO_LEDS_BIT_2, //!< =2: LED 2 (links, blau)
  LED_3 = IO_LEDS_BIT_3, //!< =3: LED 3 (rechts, blau)
  LED_4 = IO_LEDS_BIT_4  //!< =4: LED 4 (rechts, rot)
};


/*!
 * \~german
 * Initialisierung der LEDs.
 * Die Funktion muss vor Aufruf der anderen LED-Funktion aufgerufen werden um
 * das Datenrichtungs-Register (DDRB) für die Ausgabe zu konfigurieren.
 * \~english
 * This function will configure the LED bits of the data direction register DDRB for output.
 * It must be called once before a call to led_set() is made.
 */
void led_init();


/*!
 * \~german
 * Ein / Ausschalten der LEDs.
 * @param led ist die Nummer der LED (1-4) bzw. eine der Konstanten
 * @ref LED_1, @ref LED_2, @ref LED_3 oder @ref LED_4
 * @param status Beim Wert 0 wird die LED ausgeschaltet anderenfalls
 * wird die LED eingeschaltet.
 * \~english
 * Switch on/off the LEDs.
 * @param led number of the LED (1-4) (or one of the LED-constant values
 * @ref LED_1, @ref LED_2, @ref LED_3 or @ref LED_4)
 * @param status if the value is 0 the LED will be switched off, otherwise it will be switched on.
 */
void led_set(uint8_t led, uint8_t status);


/*!
 * \~german
 * Umschalten der LEDs: Ist die LED ausgeschaltet wird sie eingeschaltet, ist sie eingeschaltet wird sie ausgeschaltet.
 * @param led ist die Nummer der LED (1-4) bzw. eine der Konstanten
 * @ref LED_1, @ref LED_2, @ref LED_3 oder @ref LED_4
 * \~english
 * Toggle the state of the LEDs.
 * @param led number of the LED (1-4) (or one of the LED-constant values
 * @ref LED_1, @ref LED_2, @ref LED_3 or @ref LED_4)
 */
void led_toggle(uint8_t led);


/*!
 * \~german
 * Zustand der LED abfragen.
 * @param led ist die Nummer der LED (1-4) bzw. eine der Konstanten
 * @ref LED_1, @ref LED_2, @ref LED_3 oder @ref LED_4
 * @result Liefert den Wert 0 bei ausgeschalteter LED und den Wert 1 bei eingeschalteter LED
 * \~english
 * Get status of LED.
 * @param led number of the LED (1-4) (or one of the LED-constant values
 * @ref LED_1, @ref LED_2, @ref LED_3 or @ref LED_4)
 * @result returns 0 if the LED is switched off, otherwise it will return the value 1
 */
uint8_t led_get(uint8_t led);


/*!
 * \~german
 * Alle LEDs gleichzeitig schalten. Der Wert 0 schaltet die jeweilige LED aus, ansonsten wird sie eingeschaltetet.
 * @param led1 LED 1 (links, rot)
 * @param led2 LED 2 (links, blau)
 * @param led3 LED 3 (rechts, blau)
 * @param led4 LED 4 (rechts, rot)
 * \~english
 * Set all LEDs at once. The value 0 switches the corresponding LED off, otherwise the LED will be switched on.
 * @param led1 LED 1 (left, red)
 * @param led2 LED 2 (left, blue)
 * @param led3 LED 3 (right, blue)
 * @param led4 LED 4 (right, red)
 */
void led_setall(uint8_t led1, uint8_t led2, uint8_t led3, uint8_t led4);


#ifdef __cplusplus
} // extern "C"
#endif



#endif // _NIBOBURGER_LED_H_
