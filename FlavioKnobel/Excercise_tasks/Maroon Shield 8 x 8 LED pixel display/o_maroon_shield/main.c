
#include <niboburger/robomain.h>
#include "maroon.h"


void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
}


void maroon_welcome() {
	usart_write(
        MAROON_IMM_CLEAR()              
        MAROON_BRIGHT(*)              
        MAROON_LOAD()
		MAROON_GFX("103070ffff703010")   

        MAROON_TXBACK(".")              
	);
}


void setup() {
	
	maroon_setup(); 
	delay(200);
	maroon_welcome();
}

void loop() {
    
}


