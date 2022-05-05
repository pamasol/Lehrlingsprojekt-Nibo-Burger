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

/*! @file    storage.h
 *  @brief   Funktionen zum Abspeichern von Daten im EEPROM. 
 * Die Datensätze werden von der höchsten Addresse an im EEprom gespeichert. Jeder 
 * Datensatz hat 32 Bit (4 Byte) Kopfdaten in denen der genaue Typ (3 Byte) und die
 * Größe (1 Byte) gespeichert sind. Der Typ setzt sich aus dem Owner (Bibliothek etc...)
 * dem Eintragstyp (Liniensensor, Baudrate etc...) und der Versionsnummer zusammen.
 * Konfigurationsdaten können somit von verschiedenen Bibliotheksversionen gespeichert
 * und falls sie Kompatibel sind, gemeinsam genutzt werden. Alle Einträge sind auf 32 Bit
 * Addressen ausgerichtet (4 byte aligned).
 * STORAGE_SYS_NAME
 * STORAGE_SYS_UART0_BAUDRATE
 * 
 * 
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-03-31
 */

#ifndef _STORAGE_H_
#define _STORAGE_H_


#include <avr/io.h>


#define STORAGE_ENTRY(owner, type, version, size) ((((uint32_t)(owner))<<24) | (((uint32_t)(type))<<16) | (((uint32_t)(version))<<8) | ((uint32_t)(size)))


/* some standard system entries. Owner 0x00 - 0x7f are reserved for libraries, 0x80+ for custom purposes */
#define STORAGE_SYS_ID       0x01
#define STORAGE_USER0_ID     0x80
#define STORAGE_USER1_ID     0x81
#define STORAGE_USER2_ID     0x82
#define STORAGE_USER3_ID     0x83
#define STORAGE_USER4_ID     0x84
#define STORAGE_USER5_ID     0x85
#define STORAGE_USER6_ID     0x86
#define STORAGE_USER7_ID     0x87
#define STORAGE_USER8_ID     0x88
#define STORAGE_USER9_ID     0x89

/* entry types for system */
#define STORAGE_SYS_KEY_ID   0x01
#define STORAGE_SYS_NAME_ID  0x02
#define STORAGE_SYS_UART0_ID 0x03
#define STORAGE_SYS_UART1_ID 0x04

#define STORAGE_SYS_KEY STORAGE_ENTRY(STORAGE_SYS_ID, STORAGE_SYS_KEY_ID, 1, 4)              //!< Integer 32 Bit 
#define STORAGE_SYS_NAME STORAGE_ENTRY(STORAGE_SYS_ID, STORAGE_SYS_NAME_ID, 1, 20)           //!< String max. 20 byte
#define STORAGE_SYS_UART0_BAUDRATE STORAGE_ENTRY(STORAGE_SYS_ID, STORAGE_SYS_UART0_ID, 1, 4) //!< Integer 32 Bit 
#define STORAGE_SYS_UART1_BAUDRATE STORAGE_ENTRY(STORAGE_SYS_ID, STORAGE_SYS_UART1_ID, 1, 4) //!< Integer 32 Bit 



/**
 * copy the entry data from EEPROM to RAM
 * @param entry 32 bit entry ID
 * @param ram pointer to RAM area
 * @return true: success  / false: entry not found
 */
uint8_t storage_read(uint32_t entry, void * ram);


/**
 * copy the entry data from RAM to EEPROM
 * @param entry 32 bit entry ID
 * @param ram pointer to RAM area
 * @return true: success  / false: out of memory
 */
uint8_t storage_write(uint32_t entry, const void * ram);


/**
 * delete the entry in EEPROM
 * @param entry 32 bit entry ID
 * @return true: success  / false: entry not found
 */
uint8_t storage_delete(uint32_t entry);


/**
 * optimize data in EEPROM (TODO)
 * @return true: success  / false: structure error
 */
uint8_t storage_optimize();


/**
 * delete all data in EEPROM
 * @return true: success
 */
uint8_t storage_format();


#endif
