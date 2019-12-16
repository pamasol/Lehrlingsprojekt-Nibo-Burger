   



   /* Includes für den Zugriff auf die NIBO Burger Bibliothek */



   #include "niboburger/robomain.h"



   int LED;
   int Time;

   void setup()
   {
	   led_init();
   }

   void loop()
   {
	   for (Time=50; Time<=200; Time=Tim*2)
	   {
		   
		   
		   for (LED=1; LED<=4;LED++)
		   {
			   led_set(LED, 1);
			   delay(Time);
			   led_set(LED, 0);
			   delay(Time);
		   }
	   }
   }