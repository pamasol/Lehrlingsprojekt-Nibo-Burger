#include <niboburger/robomain.h>
#include "maroon.h"

uint16_t maroon_cnt;
uint8_t maroon_mode;

char maroon_gfxdata[] = MAROON_LOAD() MAROON_BAR("00000000");
#define MAROON_BAR_OFFSET 4

void show_sensors(uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7) {
	if (usart_txempty()) {
		maroon_gfxdata[MAROON_BAR_OFFSET+0]= getSensorChar(v0);
		maroon_gfxdata[MAROON_BAR_OFFSET+1]= getSensorChar(v1);
		maroon_gfxdata[MAROON_BAR_OFFSET+2]= getSensorChar(v2);
		maroon_gfxdata[MAROON_BAR_OFFSET+3]= getSensorChar(v3);
		maroon_gfxdata[MAROON_BAR_OFFSET+4]= getSensorChar(v4);
		maroon_gfxdata[MAROON_BAR_OFFSET+5]= getSensorChar(v5);
		maroon_gfxdata[MAROON_BAR_OFFSET+6]= getSensorChar(v6);
		maroon_gfxdata[MAROON_BAR_OFFSET+7]= getSensorChar(v7);
		usart_write(maroon_gfxdata);
	}
}

void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
}

void maroon_welcome() {
	maroon_mode=0;
	usart_write(MAROON_IMM_CLEAR() MAROON_BRIGHT(0) MAROON_LOAD() "1" MAROON_BAR("0") MAROON_DIM(*) MAROON_PAUSE(100) MAROON_DIM(5) MAROON_STIME(60) " Put me on color\n" MAROON_TXBACK("."));
}

char getSensorChar(int16_t val) {
	if (val<5)   return 'a'; // 0
	if (val<10)  return 'b'; // 1
	if (val<15)  return 'c'; // 2
	if (val<25)  return 'd'; // 3
	if (val<35)  return 'e'; // 4
	if (val<50)  return 'f'; // 5
	if (val<70)  return 'g'; // 6
	if (val<100) return 'h'; // 7
	return 'i'; // 8
}

void maroon_loop() {
	if (!usart_rxempty()) {
		char c = usart_getchar();
		if (c=='.') {
			maroon_mode = 1;
		}
	}

	if (maroon_mode) {

		if (maroon_cnt) {
			maroon_cnt--;
			return;
		}

		maroon_cnt=20;
		
		if (usart_txempty()) {
			uint16_t bl = surface_get(SURFACE_L)/8;
			uint16_t bc = surface_get(SURFACE_C)/8;
			uint16_t br = surface_get(SURFACE_R)/8;
			uint16_t bcl = surface_get(SURFACE_CL)/8;
			uint16_t bcr = surface_get(SURFACE_CR)/8;
			show_sensors(br,br,bcr,bc,bc,bcl,bl,bl);
		}
	}
}

uint8_t maroon_connected() {
	return maroon_mode;
}
