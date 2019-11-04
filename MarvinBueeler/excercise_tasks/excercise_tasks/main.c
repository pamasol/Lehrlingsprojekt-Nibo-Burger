/*
 * excercise_tasks.c
 *
 * Created: 24.09.2019 08:34:34
 * Author : Marvin Büeler
 */ 

#include "niboburger/robomain.h" 

int state;
int abstand = 20;

void setup() 
{
	led_init(); 
	analog_init(); 
	odometry_init(); 
	motpid_init(); 
	surface_readPersistent(); 
	odometry_reset();
	state = 0;
}

void loop() 
{	
	analog_wait_update();
	nibo_checkMonitorVoltage(); 
	
	unsigned long int col = surface_getColorRGB();
	
	int diff_red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
	int diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);
	int diff_blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
	int diff_green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
	int diff_yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
	
// 	led_set(1, diff_red < 8500);
// 	led_set(2, diff_yellow < 3500);
 	led_set(3, diff_blue < 4500);
 	led_set(4, diff_green < 3000);
	 
	 if (diff_red < 9000)
	 {
		 if (diff_yellow < 4000)
		 {
			 led_set(2,1);
			 led_set(1,0);
		 } 
		 else
		 {
			 led_set(1,1);
			 led_set(2,0);
		 }
	 } else
	 {
		 led_set(2,0);
		 led_set(1,0);
	 }

// 	if (diff_green < 2500)
// 	{
// 		led_set(4,1);
// 	} else
// 	{
// 		led_set(4,0);
// 	}
// 	
// 	if (diff_blue < 4000)
// 	{
// 		led_set(3,1);
// 	} 
// 	else
// 	{
// 		led_set((3,0);
// 	}
}