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

/*! @file    color.h
 *  @brief   Routinen f&uuml;r 24-Bit RGB Farben und 24-Bit HSV Farben
 */

#ifndef _NIBOBURGER_COLOR_H_
#define _NIBOBURGER_COLOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

     
enum {
  COLOR_RGB_WHITE = 0xffffffUL,
  COLOR_RGB_BLACK = 0x000000UL,
  COLOR_RGB_RED   = 0xff0000UL,
  COLOR_RGB_GREEN = 0x00ff00UL,
  COLOR_RGB_BLUE  = 0x0000ffUL,

  COLOR_RGB_CAL_BLACK  = 0x000000UL,
  COLOR_RGB_CAL_WHITE  = 0xffffffUL,
  COLOR_RGB_CAL_BLUE   = 0x220077UL,
  COLOR_RGB_CAL_RED    = 0xee1122UL,
  COLOR_RGB_CAL_GREEN  = 0x668844UL,
  COLOR_RGB_CAL_YELLOW = 0xee5555UL
};


enum {
  COLOR_HSV_CAL_BLACK  = 0x000000UL,
  COLOR_HSV_CAL_WHITE  = 0x0000ffUL,
  COLOR_HSV_CAL_BLUE   = 0x91b558UL,
  COLOR_HSV_CAL_RED    = 0xbaf4b7UL,
  COLOR_HSV_CAL_GREEN  = 0x27cfaaUL,
  COLOR_HSV_CAL_YELLOW = 0x0cebfcUL
};  

/*!
 * \~german
 * Farbdifferenz im RGB-Modell berechnen. Liefert die Summe der Quadrate der Differzen zurück: (rA-rB)²+(gA-gB)²+(bA-bB)²
 * @param colorA Farbe A
 * @param colorA Farbe B
 * @result Summe der Quadrate der Differzen
 * \~english
 * calculate color difference in RGB model. Returns sum of sqares of deltas: (rA-rB)²+(gA-gB)²+(bA-bB)²
 * @param colorA color A
 * @param colorA color B
 * @result sum of sqares of deltas
 */
uint16_t color_diff_rgb(uint32_t colorA, uint32_t colorB);

/*!
 * \~german
 * Farbdifferenz im HSV-Modell berechnen. Liefert die gewichtete Summe der Quadrate der Differenzen zurück.
 * @param colorA Farbe A
 * @param colorA Farbe B
 * @result Summe der Quadrate der Differzen
 * \~english
 * calculate color difference in HSV model. Returns weighted sum of sqares of deltas.
 * @param colorA color A
 * @param colorA color B
 * @result sum of sqares of deltas
 */
uint16_t color_diff_hsv(uint32_t colorA, uint32_t colorB);

/*!
 * \~german
 * Farbdifferenz im HSV-Modell berechnen. Liefert die gewichtete Summe der Quadrate der Differenzen zurück.
 * @param colorA Farbe A
 * @param colorA Farbe B
 * @param vTol zusätzliche Toleranz für Helligkeit
 * @result Summe der Quadrate der Differzen
 * \~english
 * calculate color difference in HSV model. Returns weighted sum of sqares of deltas.
 * @param colorA color A
 * @param colorA color B
 * @param vTol additional tolerance for value
 * @result sum of sqares of deltas
 */
uint16_t color_diff_hsv_bal(uint32_t colorA, uint32_t colorB, uint8_t vTol);

/*!
 * \~german
 * 24 Bit HSV Wert aus R, G und B Komponente berechnen.
 * \~english
 * calculate 24 bit HSV value out of R, G and B component.
 */
uint32_t color_convert_RGB2hsv(uint8_t r, uint8_t g, uint8_t b);

/*!
 * \~german
 * 24 Bit RGB Wert aus R, G und B Komponente berechnen.
 * \~english
 * calculate 24 bit RGB value out of R, G and B component.
 */
uint32_t color_convert_RGB2rgb(uint8_t r, uint8_t g, uint8_t b);


#ifdef __cplusplus
}
#endif

#endif