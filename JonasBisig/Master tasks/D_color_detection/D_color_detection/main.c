

#include "niboburger/robomain.h"


void setup() {

    led_init();
    analog_init();
	surface_init();
    

}

void loop() {
	
	unsigned long int col = surface_getColorRGB();
	
	int diff_blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
	int diff_green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
	int diff_red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
	int diff_yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
	//int diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK);
	
	
	led_set(3, diff_blue < 4500);
	led_set(4, diff_green < 3000);
	
	if (diff_red < 10000)
	{
		if (diff_yellow < 4000)
		{
			led_set(2,1);
			led_set(1,0);
		}else
		{
			led_set(1,1);
			led_set(2,0);
			
		}
		
	}else
	{
		led_set(2,0);
		led_set(1,0);
	}
	
    
}
