/* Includes für den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"
int LED;
void setup()
{
led_init();
}

void loop()
{
for (LED=2; LED<=4;LED++)
{
led_set(LED, 1);
delay(500);
led_set(LED, 0);
delay(500);
}
}