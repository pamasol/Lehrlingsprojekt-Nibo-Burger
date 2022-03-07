
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
        "1"
        MAROON_BAR("0")                
                                        
                                        
        MAROON_PAUSE(1000)              
        MAROON_STIME(3000)              
		" Pamasol\n"
        MAROON_LOAD()                   
                                        
        MAROON_GFX("ffe7e78181e7e7ff")  
                                        // ff = 11111111
                                        // e7 = 11100111
                                        // e7 = 11100111
                                        // 81 = 10000001
                                        // 81 = 10000001
                                        // e7 = 11100111
                                        // e7 = 11100111
                                        // ff = 11111111
        MAROON_TXBACK(".")              
	);
}


void setup() {
	led_init();
	led_setall(1, 1, 1, 1);
	delay(200);

	maroon_setup(); 
	delay(200);
	maroon_welcome();
}

void loop() {
    
}


