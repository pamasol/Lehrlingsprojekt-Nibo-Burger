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

/*! @file    bgx1_protocol.h
 *  @brief   Definitionen für die Kommunikation des BGX1 über I²C.
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2010-06-22
 *
 */

#ifndef NIBOBURGER_BGX1_PROTOCOL_H_
#define NIBOBURGER_BGX1_PROTOCOL_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif


#define BGX1_VERSION 0x0103


enum {
  // System:
  CMD_Reset           = 0x00, // () -> ()
  CMD_GetVersion      = 0x01, // () -> (uint16_t version)
  CMD_GetStatus       = 0x02, // () -> (uint8_t status)
  CMD_SetStatus       = 0x03, // (uint8_t status) -> ()
  //CMD_ReadEEPROM      = 0x04, // (uint16_t address, uint8_t size) -> (uint8_t[] data)
  //CMD_WriteEEPROM     = 0x05, // (uint16_t address, uint8_t size, uint8_t[] data) -> ()
  
  // Display:
  CMD_Move            = 0x10, // (uint8_t x, uint8_t y) -> ()
  CMD_Mode            = 0x11, // (uint8_t mode) -> ()
  CMD_FillAll         = 0x12, // (uint8_t pattern) -> ()
  CMD_Print           = 0x13, // (char text[]) -> (uint8_t x, uint8_t y)
  CMD_TextWidth       = 0x14, // (char text[]) -> (uint8_t width)
  CMD_SelectFont      = 0x15, // (uint8_t id) -> ()
  CMD_HLine           = 0x16, // (uint8_t length) -> (uint8_t x, uint8_t y)
  CMD_VLine           = 0x17, // (uint8_t length) -> (uint8_t x, uint8_t y)
  CMD_Box             = 0x18, // (uint8_t width, uint8_t height) -> (uint8_t x, uint8_t y)
  CMD_Bitmap          = 0x19, // (uint8_t width, uint8_t height, uint8_t bitmap[]) -> (uint8_t x, uint8_t y)
  CMD_EmbeddedImage   = 0x1a, // (uint8_t id) -> (uint8_t x, uint8_t y)
  CMD_LineTo          = 0x1b, // (uint8_t x, uint8_t y) -> ()

  // Terminal:
  CMD_TermClear       = 0x30, // () -> ()
  CMD_TermGoto        = 0x31, // (uint8_t col, uint8_t row) -> ()
  CMD_TermScroll      = 0x32, // (uint8_t lines) -> ()
  CMD_TermPrint       = 0x33, // (char text[]) -> ()

  // IO:
  CMD_SyncPort        = 0x40, // (uint8_t ddr, uint8_t port) -> (uint8_t pin)
  CMD_GetAnalog       = 0x41, // (uint8_t idx) -> (uint16_t value)
  CMD_SyncInterface   = 0x42, // (uint8_t leds) -> (uint8_t keys)
  CMD_SetIllumination = 0x43, // (uint16_t value) -> ()
  
  CMD_INVALID         = 0xff 
};


#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBOBURGER_BGX1_PROTOCOL_H_
