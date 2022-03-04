#include <niboburger/robomain.h>

void setup() 
{
led_init();
}

void loop() 
{
led_set (1, 1);
delay (500);
led_set (1, 0);
delay (500);
}