   



   /* Includes für den Zugriff auf die NIBO Burger Bibliothek */



   #include "niboburger/robomain.h"



   int LED;

   void setup()
   {
	led_init();
	analog_init();
	odometry_init();
   }

   void loop()
   {
char key=key_get_char();

if (key == 'A')
{
odometry_resetLeft();
}
if (key == 'B')
{
odometry_resetRight();
}
if (key == 'C')
{
odometry_reset();
}
led_set(1, odometry_getLeft(0)>10);
led_set(2, odometry_getLeft(0)>20);
led_set(3, odometry_getRight(0)>10);
led_set(4, odometry_getRight(0)>20);
   }