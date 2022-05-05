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

/*! @file       bgx1.c
 * @brief       Modul zur Kommunikation mit der Display-Erweiterung BGX1
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 * @date        2010-03-28
*/

#include "niboburger/iodefs.h"
#include "niboburger/bgx1.h"
#include "niboburger/bgx1_protocol.h"
#include "niboburger/base.h"
#include "niboburger/i2cmaster.h"
#include <string.h>
#include <avr/pgmspace.h>

#define BGX1_I2C_ADDR (11<<1)

#define BITMAP_MAX 18

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
//#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)


#ifdef __cplusplus
extern "C" {
#endif

uint8_t bgx1_DDR = 0x00;
uint8_t bgx1_PORT = 0x00;
uint8_t bgx1_PIN = 0x00;
uint8_t bgx1_Leds = 0x00;
uint8_t bgx1_Keys = 0x00;

uint8_t bgx1_x;
uint8_t bgx1_y;


static uint8_t bgx1_transaction(uint8_t txsize, uint8_t rxsize) {
  i2c_tx_size = txsize;
  i2c_rx_size = rxsize;
  i2c_start_transmission();
  return i2c_wait_transmission();
}

static uint8_t bgx1_transaction_posres(uint8_t txsize, uint8_t rxsize) {
  uint8_t res = bgx1_transaction(txsize, rxsize);
  if (i2c_status() == I2C_SUCCESS) {
    bgx1_x = i2c_buf[1];
    bgx1_y = i2c_buf[2];
  }
  return res;
}


uint8_t bgx1_init() {
  if (!(nibo_initialization&NIBO_I2C_INITIALIZED)) {
    i2c_init();
  }
  uint16_t version = bgx1_getVersion();
  return (i2c_status() == I2C_SUCCESS) && (HIBYTE(version) == HIBYTE(BGX1_VERSION));
}

void bgx1_reset() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Reset;
  bgx1_transaction(2, 1);
}

uint16_t bgx1_getVersion() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_GetVersion;
  bgx1_transaction(2, 3);
  return MAKE_WORD(i2c_buf[1], i2c_buf[2]);
}

uint8_t bgx1_getStatus() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_GetStatus;
  bgx1_transaction(2, 2);
  return i2c_buf[1];
}

void bgx1_setStatus(uint8_t status) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_SetStatus;
  i2c_buf[2] = status;
  bgx1_transaction(3, 1);
}

void bgx1_move(uint8_t x, uint8_t y) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Move;
  i2c_buf[2] = x;
  i2c_buf[3] = y;
  bgx1_transaction(4, 1);
}

void bgx1_mode(uint8_t mode) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Mode;
  i2c_buf[2] = mode;
  bgx1_transaction(3, 1);
}

void bgx1_fillAll(uint8_t pattern) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_FillAll;
  i2c_buf[2] = pattern;
  bgx1_transaction(3, 1);
}

void bgx1_print(const char * text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Print;
  uint8_t len = strlen(text);
  memcpy(i2c_buf+2, text, len);
  bgx1_transaction_posres(2+len, 3);
}

void bgx1_print_P(const char * text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Print;
  uint8_t len = strlen_P(text);
  memcpy_P(i2c_buf+2, text, len);
  bgx1_transaction_posres(2+len, 3);
}

uint8_t bgx1_textWidth(const char * text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TextWidth;
  uint8_t len = strlen(text);
  memcpy(i2c_buf+2, text, len);
  bgx1_transaction(2+len, 2);
  return i2c_buf[1];
}

uint8_t bgx1_textWidth_P(PGM_P text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TextWidth;
  uint8_t len = strlen_P(text);
  memcpy_P(i2c_buf+2, text, len);
  bgx1_transaction(2+len, 2);
  return i2c_buf[1];
}

void bgx1_selectFont(uint8_t id) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_SelectFont;
  i2c_buf[2] = id;
  bgx1_transaction(3, 1);
}

void bgx1_hLine(uint8_t length) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_HLine;
  i2c_buf[2] = length;
  bgx1_transaction_posres(3, 3);
}

void bgx1_vLine(uint8_t length) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_VLine;
  i2c_buf[2] = length;
  bgx1_transaction_posres(3, 3);
}

void bgx1_box(uint8_t width, uint8_t height) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Box;
  i2c_buf[2] = width;
  i2c_buf[3] = height;
  bgx1_transaction_posres(4, 3);
}

// max 18 Byte bitmap
static void bgx1_drawTile(uint8_t width, uint8_t height, const uint8_t bitmap[]) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Bitmap;
  i2c_buf[2] = width;
  i2c_buf[3] = height;
  uint8_t size = height * ((width-1)/8+1);
  memcpy (i2c_buf+4, bitmap, size);
  bgx1_transaction_posres(4+size, 3);
}

// max 18 Byte bitmap
static void bgx1_drawTile_P(uint8_t width, uint8_t height, PGM_P bitmap) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_Bitmap;
  i2c_buf[2] = width;
  i2c_buf[3] = height;
  uint8_t size = height * ((width-1)/8+1);
  memcpy_P (i2c_buf+4, bitmap, size);
  bgx1_transaction_posres(4+size, 3);
}


void bgx1_drawBitmap(uint8_t width, uint8_t height, const uint8_t bitmap[]) {
  const uint8_t * data = bitmap;
  uint8_t row_size = ((width-1)/8+1);
  uint8_t rows_per_tile = BITMAP_MAX/row_size;
  uint8_t x = bgx1_y;
  uint8_t y = bgx1_x;
  while (height) {
    bgx1_move(x, y);
    uint8_t h = (height>rows_per_tile)?rows_per_tile:height;
    bgx1_drawTile(width, h, data);
    while (h--) {
      data += row_size;
      height--;
      y++;
    }
  }  
}

void bgx1_drawBitmap_P(uint8_t width, uint8_t height, PGM_P bitmap) {
  PGM_P data = bitmap;
  uint8_t row_size = ((width-1)/8+1);
  uint8_t rows_per_tile = BITMAP_MAX/row_size;
  uint8_t x = bgx1_y;
  uint8_t y = bgx1_x;
  while (height) {
    bgx1_move(x, y);
    uint8_t h = (height>rows_per_tile)?rows_per_tile:height;
    bgx1_drawTile_P(width, h, data);
    while (h--) {
      data += row_size;
      height--;
      y++;
    }
  }  
}

void bgx1_embeddedImage(uint8_t id) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_EmbeddedImage;
  i2c_buf[2] = id;
  bgx1_transaction_posres(3, 3);
}

void bgx1_lineTo(uint8_t x, uint8_t y) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_LineTo;
  i2c_buf[2] = x;
  i2c_buf[3] = y;
  bgx1_transaction_posres(4, 3);
}

void bgx1_termClear() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TermClear;
  bgx1_transaction(2, 1);
}

void bgx1_termGoto(uint8_t col, uint8_t row) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TermGoto;
  i2c_buf[2] = col;
  i2c_buf[3] = row;
  bgx1_transaction(4, 1);
}

void bgx1_termScroll(int8_t lines) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TermScroll;
  i2c_buf[2] = lines;
  bgx1_transaction(3, 1);
}

void bgx1_termPrint(const char * text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TermPrint;
  uint8_t len = strlen(text);
  memcpy (i2c_buf+2, text, len);
  bgx1_transaction(2+len, 2);
}

void bgx1_termPrint_P(PGM_P text) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_TermPrint;
  uint8_t len = strlen_P(text);
  memcpy_P (i2c_buf+2, text, len);
  bgx1_transaction(2+len, 2);
}

void bgx1_syncPort() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_SyncPort;
  i2c_buf[2] = bgx1_DDR;
  i2c_buf[3] = bgx1_PORT;
  bgx1_transaction(4, 2);
  bgx1_PIN = i2c_buf[1];
}

uint16_t bgx1_getAnalog(uint8_t index) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_GetAnalog;
  i2c_buf[2] = index;
  bgx1_transaction(3, 3);
  return MAKE_WORD(i2c_buf[1], i2c_buf[2]);
}

void bgx1_syncInterface() {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_SyncInterface;
  i2c_buf[2] = bgx1_Leds;
  bgx1_transaction(3, 2);
  bgx1_Keys = i2c_buf[1];
}

void bgx1_setIllumination(uint16_t value) {
  i2c_buf[0] = BGX1_I2C_ADDR;
  i2c_buf[1] = CMD_SetIllumination;
  i2c_buf[2] = HIBYTE(value);
  i2c_buf[3] = LOBYTE(value);
  bgx1_transaction(4, 1);
}




#ifdef __cplusplus
} // extern "C"
#endif
