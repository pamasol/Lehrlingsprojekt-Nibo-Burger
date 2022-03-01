#ifndef MAROON_H_
#define MAROON_H_

#define MAROON_CLEAR() "\33c"
#define MAROON_BRIGHT(x) "\33b" #x
#define MAROON_DIM(x) "\33d" #x
#define MAROON_TXBACK(c) "\33t" c
#define MAROON_LOAD() "\33l"

#define MAROON_FONT_ECO() "\33e1"
#define MAROON_FONT_NORM() "\33e0"
#define MAROON_FONT_FIXED() "\33p0"
#define MAROON_FONT_PROP() "\33p1"

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

uint8_t maroon_connected();

#endif /* MAROON_H_ */