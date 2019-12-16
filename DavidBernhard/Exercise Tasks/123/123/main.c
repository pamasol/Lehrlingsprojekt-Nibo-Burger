   



   /* Includes für den Zugriff auf die NIBO Burger Bibliothek */



   #include "niboburger/robomain.h"


   int Power;
   int LED;

   void setup()
   {
	led_init();
	analog_init();
	motpwm_init();
	Power = 0;
   }

   void loop()
   {
char key=key_get_char();

switch (key)
   {
case 'a':

Power= Power + 200;
break;

case 'B':
Power = 0;
break;

case 'c':
Power = Power - 200;
break;

   }
	motpwm_setLeft(Power);
	motpwm_setRight(Power);
	delay(10);
   }