/*  BSD-License:

Copyright (c) 2011 by Nils Springob, nicai-systems, Germany

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

/*! @file    rc5.h
 *  @brief   Routinen fuer die Dekodierung von RC5-Fernbedienungs-Codes
 *  Der SFH5110-36 (oder kompatibel) kann einfach mittig in die Buchsenleiste X13 gesteckt 
 *  werden (Blickrichtung nach hinten / umgebogen nach oben)
 *  Als Sendedioden werden einfach die IR-Sensorbricks in den Slots FLL und FRR verwendet.
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2016-01-10
*/
#ifndef _NIBOBURGER_RC5_H_
#define _NIBOBURGER_RC5_H_

#include "niboburger/iodefs.h"

extern uint8_t ir_state;


/*!
 * RC5-Code senden
 * @param code zu übertragener RC5 Code.
 */
void rc5_tx_start(uint16_t code);

/*!
 * Aktuelle Übertragung abbrechen
 */
void rc5_tx_abort();

/*!
 * Empfang von RC5 Codes ermöglichen
 */
void rc5_rx_enable();

/*!
 * Empfang von RC5 Codes beenden
 */
void rc5_rx_disable();

/*!
 * RC5-Daten lesen
 * @return Empfangener RC5 Code, oder 0 falls seit dem letzten Aufruf kein neuer Code übertragen wurde.
 */
uint16_t rc5_rx_read();


#endif // _NIBOBURGER_RC5_H_
