   



   /* Includes f�r den Zugriff auf die NIBO Burger Bibliothek */



   #include "niboburger/robomain.h"



   int LED;

   void setup()
   {
	   led_init();
	analog_init();
   }

   void loop()
   {
char key=key_get_char();
switch (key)
   {

case 'A':
led_set(1,1);
led_set(4,1);
break;

case 'B':
led_set(2,1);
led_set(3,1);
break;

case 'C':
led_setall(0,0,0,0);
break;
}
   }