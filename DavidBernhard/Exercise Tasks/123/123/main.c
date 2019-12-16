   



   /* Includes für den Zugriff auf die NIBO Burger Bibliothek */



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
led_set(3,1);
break;
case 'a':
led_set(3,0);
break;
}
   }