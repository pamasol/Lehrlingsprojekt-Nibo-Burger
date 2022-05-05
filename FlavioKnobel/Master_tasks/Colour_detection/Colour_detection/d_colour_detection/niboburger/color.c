
#include <stdlib.h>

#include "niboburger/iodefs.h"
#include "niboburger/color.h"
#include "niboburger/utils.h"

#define SQARE16(x) (((int16_t)x)*x)
#define SQARE32(x) (((int32_t)x)*x)

uint16_t color_diff_rgb(uint32_t colorA, uint32_t colorB) {
  uint16_t r = absdiff( ((uint8_t)((colorB >> 16) & 0xff)) , ((uint8_t)((colorA >> 16) & 0xff)) );
  uint16_t g = absdiff( ((uint8_t)((colorB >>  8) & 0xff)) , ((uint8_t)((colorA >>  8) & 0xff)) );
  uint16_t b = absdiff( ((uint8_t)((colorB >>  0) & 0xff)) , ((uint8_t)((colorA >>  0) & 0xff)) );
  uint32_t res = 0;
  res += r*r;
  res += g*g;
  res += b*b;
  if (res>INT16_MAX) {
    res = INT16_MAX;
  }
  return res;
}


uint16_t color_diff_hsv(uint32_t colorA, uint32_t colorB) {
  return color_diff_hsv_bal(colorA, colorB, 0);
}

uint16_t color_diff_hsv_bal(uint32_t colorA, uint32_t colorB, uint8_t vTol) {
  uint8_t hA = (int8_t)((colorA >> 16) & 0xff); // [0..255]
  uint8_t sA = (int8_t)((colorA >>  8) & 0xff); // [0..255]
  uint8_t vA = (int8_t)((colorA >>  0) & 0xff); // [0..255]
  uint8_t hB = (int8_t)((colorB >> 16) & 0xff); // [0..255]
  uint8_t sB = (int8_t)((colorB >>  8) & 0xff); // [0..255]
  uint8_t vB = (int8_t)((colorB >>  0) & 0xff); // [0..255]
  
  uint8_t v0 = ((uint16_t)vA+vB)/2; // [0..255]
  uint8_t s0 = ((uint16_t)sA+sB)/2; // [0..255]
    
  uint8_t dh = abs((int8_t)(hA-hB)); // [0..128]
  uint8_t ds = absdiff(sA, sB); // [0..255]
  uint8_t dv = absdiff(vA, vB); // [0..255]
  
  if (dv>vTol) {
    dv -= vTol;
  } else {
    dv = 0;
  }
  
  uint32_t diff = SQARE16(dh);
  diff *= s0;
  diff /= 256;
  diff += SQARE16(ds);
  diff *= v0;
  diff /= 256;
  diff += SQARE16(dv);
  if (diff>INT16_MAX) {
    diff = INT16_MAX;
  }
  return diff;
}

uint32_t color_convert_RGB2hsv(uint8_t r, uint8_t g, uint8_t b) {
  // r, g, b [0..255]

  uint8_t  v = max3(r,g,b);     // [0..255] // = MAX(R,G,B)
  uint16_t d = v - min3(r,g,b); // [0..255] // = MAX(R,G,B) - MIN(R,G,B)
  
  uint8_t  s = v?((255U*d)/v):0; // [0..255] // = 255 * (MAX(R,G,B) - MIN(R,G,B)) / MAX(R,G,B)

  int16_t  h = 0;
  
  if (d) {
    if (v==r) {
      h = (85 * ((int16_t)g-(int16_t)b)) / (d+d) + 0;
      if (h<0) {
        h += 256;
      }
    } else if (v==g) {
      h = (85 * ((int16_t)b-(int16_t)r)) / (d+d) + 85;
    } else if (v==b) {
      h = (85 * ((int16_t)r-(int16_t)g)) / (d+d) + 171;
    }
  }
  
  // h [0..255]
  
  return ((uint32_t)(uint8_t)h) << 16 |
         ((uint32_t)s) << 8  |
         ((uint32_t)v);
}

uint32_t color_convert_RGB2rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r) << 16 |
         ((uint32_t)g) << 8  |
         ((uint32_t)b);
}

