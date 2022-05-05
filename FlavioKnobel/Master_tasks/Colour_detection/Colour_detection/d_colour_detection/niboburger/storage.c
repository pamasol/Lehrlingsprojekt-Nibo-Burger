
#include "niboburger/storage.h"
#include <avr/eeprom.h>

#include <avr/version.h>

#if (__AVR_LIBC_VERSION__<10607UL)
static inline void eeprom_update_block (const void *__src, void *__dst, size_t __n) {
  eeprom_write_block (__src, __dst, __n);
}

static inline void eeprom_update_dword (uint32_t *__p, uint32_t __value) {
  eeprom_write_dword (__p, __value);
}
#endif


uint8_t storage_read(uint32_t entry, void * ram) {
  uint16_t eepos = E2END & 0xfffc;

  while (1) {
    uint32_t data=eeprom_read_dword((const uint32_t *)eepos);
    uint8_t size = data & 0x000000ff;

    if ((size==0xff)||(size==0x00)) {
      return 0;
    }

    if (size&0x03) {
       size &= 0xfc;
       size += 4;
    }

    eepos -= size;

    if (data==entry) {
      eeprom_read_block (ram, (const void *)eepos, data & 0x000000ff);
      return 1;
    }

    eepos -= 4;

    if (eepos>E2END) {
      return 0;
    }
  }
}




uint8_t storage_write(uint32_t entry, const void * ram) {
  uint16_t eepos = E2END & 0xfffc;

  while (1) {
    uint32_t data=eeprom_read_dword((const uint32_t *)eepos);
    uint8_t size = data & 0x000000ff;

    if ((size==0xff)) {
      if (eepos < (entry&0xff)) {
        // out of memory
        return 0;
      }
      eeprom_write_dword((uint32_t *)eepos, entry);
      data = entry;
      size = data & 0x000000ff;
    }

    if ((size==0xff)||(size==0x00)) {
      return 0;
    }

    if (size&0x03) {
       size &= 0xfc;
       size += 4;
    }

    eepos -= size;

    if (data==entry) {
      eeprom_update_block (ram, (void *)eepos, data & 0x000000ff);
      return 1;
    }

    eepos -= 4;

    if (eepos>E2END) {
      return 0;
    }
  }
}



uint8_t storage_delete(uint32_t entry) {
  uint16_t eepos = E2END & 0xfffc;

  while (1) {
    uint32_t data=eeprom_read_dword((const uint32_t *)eepos);
    uint8_t size = data & 0x000000ff;

    if ((size==0xff)||(size==0x00)) {
      return 0;
    }

    if (size&0x03) {
       size &= 0xfc;
       size += 4;
    }

    if (data==entry) {
      data |= 0xff000000UL;
      eeprom_update_dword ((uint32_t *)eepos, data);
      return 1;
    }

    eepos -= size;

    eepos -= 4;

    if (eepos>E2END) {
      return 0;
    }
  }
}


uint8_t storage_format() {
  uint16_t eepos = E2END & 0xfffc;
  while (eepos) {
    eeprom_update_dword ((uint32_t *)eepos, 0xffffffff);
    eepos -= 4;
  }
  return 1;
}

