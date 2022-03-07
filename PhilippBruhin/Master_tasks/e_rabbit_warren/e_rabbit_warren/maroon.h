#ifndef MAROON_H_
#define MAROON_H_

#define MAROON_IMM_CLEAR() "\20c"
#define MAROON_BRIGHT(x) "\33b" #x
#define MAROON_LOAD() "\33l"
#define MAROON_BAR(bar) "\33B" bar " "
#define MAROON_GFX(gfx) "\33G" gfx " "
#define MAROON_DIM(x) "\33d" #x
#define MAROON_PAUSE(x) "\33P" #x " "
#define MAROON_STIME(x) "\33S" #x " "
#define MAROON_TXBACK(c) "\33t" c

void maroon_setup();

void maroon_welcome();

void maroon_print(char str[]);

void display_ArrowL();
void display_ArrowR();
void display_ArrowD();
void display_X();

uint8_t maroon_connected();

#endif /* MAROON_H_ */