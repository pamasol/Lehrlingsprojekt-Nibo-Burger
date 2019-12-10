#ifndef _MAROON_H_
#define _MAROON_H_

#define MAROON_CLEAR() "\33c"
#define MAROON_BRIGHT(x) "\33b" #x
#define MAROON_DIM(x) "\33d" #x
#define MAROON_TXBACK(c) "\33t" c
#define MAROON_LOAD() "\33l"

#define MAROON_BAR(bar) "\33B" bar " "
#define MAROON_GFX(gfx) "\33G" gfx " "
#define MAROON_PAUSE(x) "\33P" #x " "
#define MAROON_STIME(x) "\33S" #x " "
#define MAROON_DTIME(x) "\33D" #x " "
#define MAROON_TRANSITION(n, x) "\33T" #n #x " "

#define MAROON_IMM_CLEAR() "\20c"
#define MAROON_IMM_DIM(x) "\20d" #x

void maroon_setup();

void maroon_welcome();

void maroon_loop();

void maroon_bars(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h);

uint8_t maroon_connected();

#endif
