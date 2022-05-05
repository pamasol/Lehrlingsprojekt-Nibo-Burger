/*  BSD-License:

Copyright (c) 2010 by Nils Springob, nicai-systems, Germany

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

/*! @file    bgx1.h
 *  @brief   Routinen zur Kommunikation mit der Display-Erweiterung BGX1 über I²C.
 *  Die Erweiterung für den NIBObee ist elektronisch kompatibel, wird jedoch zur
 *  Zeit nicht weiter unterstützt!
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2010-03-28
 *
 */

#ifndef NIBOBURGER_BGX1_H_
#define NIBOBURGER_BGX1_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Initialisierung des BGX1
 * Ruft bei Bedarf i2c_init() auf.
 * @return 0 im Fehlerfall
 */
uint8_t bgx1_init();


/*!
 * Setzt das Display zurück.
 */
void bgx1_reset();

/*!
 * Liefert die aktuelle Software-Version zurück.
 */
uint16_t bgx1_getVersion();

/*!
 * Liefert die Statusvariable zurück. 
 * @return 0xff nach dem Einschalten, 0x00 nach dem Reset, ansonsten der Wert, der mit bgx1_setStatus gesetzt wurde.
 */
uint8_t bgx1_getStatus();

/*!
 * Setzt die Statusvariable.
 */
void bgx1_setStatus(uint8_t);

/*!
 * Setzt Grafikcursor auf angegebene Position
 * @param x X-Position des Cursors (0..127)
 * @param y Y-Position des Cursors (0..63)
 */
void bgx1_move(uint8_t x, uint8_t y);

/*!
 * Wählt den Grafikmodus
 * @param mode Grafikmodus
 */
void bgx1_mode(uint8_t mode);

/*!
 * Fulle gesamten Bildschirm mit angegebener Bitfolge
 * @param val Bitmaske
 */
void bgx1_fillAll(uint8_t pattern);

/*!
 * Gibt angegebenen Nullterminierten String an der aktuellen Grafikposition aus.
 * @param txt Nullterminierter String im RAM
 */
void bgx1_print(const char * text);

/*!
 * Gibt angegebenen Nullterminierten String an der aktuellen Grafikposition aus.
 * @param txt Nullterminierter String im FLASH
 */
void bgx1_print_P(const char * text);

/*!
 * Liefert Breite in Pixeln für Nullterminierten String
 * @param txt Nullterminierter String im RAM
 * @return Breite in Pixeln
 */
uint8_t bgx1_textWidth(const char * text);

/*!
 * Liefert Breite in Pixeln für Nullterminierten String
 * @param txt Nullterminierter String im FLASH
 * @return Breite in Pixeln
 */
uint8_t bgx1_textWidth_P(const char * text);

/*!
 * Wählt die Schriftart.
 * @param font 0=fix, 1=Proportional
 */
void bgx1_selectFont(uint8_t font);

/*!
 * Zeichnet eine horizontale Linie von der aktuellen Position nach rechts
 * @param length Länge der Linie
 */
void bgx1_hLine(uint8_t length);

/*!
 * Zeichnet eine vertikale Linie von der aktuellen Position nach unten
 * @param length Länge der Linie
 */
void bgx1_vLine(uint8_t length);

/*!
 * Zeichnet ein Rechteck.
 * @param width Breite
 * @param height Höhe
 */
void bgx1_box(uint8_t width, uint8_t height);

/*!
 * Gibt angegebene Bitmap an der aktuellen Grafikposition aus.
 * Die bitmap befindet sich dabei im RAM
 */
void bgx1_drawBitmap(uint8_t width, uint8_t height, const uint8_t bitmap[]);

/*!
 * Gibt angegebene Bitmap an der aktuellen Grafikposition aus.
 * Die bitmap befindet sich dabei im Flash
 */
void bgx1_drawBitmap_P(uint8_t width, uint8_t height, PGM_P bitmap);

/*!
 * Zeichnet eine Linie zur angegebenen Position.
 * @param x X-Position des Zielpunktes (0..127)
 * @param y Y-Position des Zielpunktes (0..63)
 */
void bgx1_lineTo(uint8_t x, uint8_t y);

/*!
 * Löscht den Bildschirm und setzt den Terminal-Cursor an die Position (0/0).
 */
void bgx1_termClear();

/*!
 * Setzt den Terminal-Cursor an die angegebene Position.
 * @param col Spalte
 * @param col Zeile
 */
void bgx1_termGoto(uint8_t col, uint8_t row);

/*!
 * Scrollt den Bildschirm um die angegebene Zeilenanzahl.
 * @param lines Anzahl der Zeilen
 */
void bgx1_termScroll(int8_t lines);

/*!
 * Gibt den Text im Terminal-Modus aus.
 * @param text Nullterminierter String im RAM
 */
void bgx1_termPrint(const char * text);

/*!
 * Gibt den Text im Terminal-Modus aus.
 * @param text Nullterminierter String im FLASH
 */
void bgx1_termPrint_P(PGM_P text);


/*!
 * DDR Register des Erweiterungsports am BGX1, wird durch bgx1_syncPort() an das BGX1 weitergeleitet.
 */
extern uint8_t bgx1_DDR;

/*!
 * PORT Register des Erweiterungsports am BGX1, wird durch bgx1_syncPort() an das BGX1 weitergeleitet.
 */
extern uint8_t bgx1_PORT;

/*!
 * PIN Register des Erweiterungsports am BGX1, wird durch bgx1_syncPort() mit dem Wert des BGX1 aktualisiert.
 */
extern uint8_t bgx1_PIN;

/*!
 * Synchronisiert Port Bits.
 * Die Variablen bgx1_DDR, bgx1_PORT und bgx1_PIN werden mit dem Registern des BGX1 synchronisisert.
 */
void bgx1_syncPort();

/*!
 * Der analoge Eingang wird gemessen und der Wert zurückgeliefert.
 */
uint16_t bgx1_getAnalog(uint8_t index);


/*!
 * Leds Register des BGX1, wird durch bgx1_syncInterface() an das BGX1 weitergeleitet.
 */
extern uint8_t bgx1_Leds;

/*!
 * Keys Register des BGX1, wird durch bgx1_syncInterface() mit dem Wert des BGX1 aktualisiert.
 */
extern uint8_t bgx1_Keys;

/*!
 * Synchronisert Leds und Keys.
 * Die Variablen bgx1_Leds und bgx1_Keys werden mit dem BGX1 synchronisisert.
 */
void bgx1_syncInterface();

/*!
 * Wert für Displaybeleuchtung setzen
 * @param light Helligkeit (0..1024)
 */
void bgx1_setIllumination(uint16_t illu);




#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBOBURGER_BGX1_H_
