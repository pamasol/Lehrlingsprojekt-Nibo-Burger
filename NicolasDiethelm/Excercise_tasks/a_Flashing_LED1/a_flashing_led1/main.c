/*
 * a_flashing_led1.c
 *
 * Created: 03.03.2022 12:10:14
 * Author : nicol
 */ 
#include <niboburger/robomain.h>

void setup() {
	led_init();
}

void loop() {
	led_set(2, 1);
	delay(500);
	led_set(2,0);
	delay(500);
}

