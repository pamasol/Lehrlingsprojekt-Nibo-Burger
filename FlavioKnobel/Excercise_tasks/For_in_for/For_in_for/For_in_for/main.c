#include "niboburger/robomain.h"

void setup()
{
led_init();
}

void loop()
{
int LedNr, Wart;


for(Wart=50; Wart<=200; Wart*=2)
{
for(LedNr=1; LedNr<=4;LedNr++)
{
led_set(LedNr,1);
delay(Wart);
led_set(LedNr,0);
delay(Wart);
}
}
}