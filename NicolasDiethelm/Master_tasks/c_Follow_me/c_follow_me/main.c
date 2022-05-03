
	#include <niboburger/robomain.h>

int speedl = 0;
int speedr = 0;
int State = 0;

void setup() {
	analog_init();
	led_init();
	motpwm_init();
	usart_setbaudrate(38400);
	usart_enable();
	
	usart_write("1");
	
}

void loop() {
	
	
	
	analog_wait_update();
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	char key =key_get_char();
	
	
	switch (key)
	{
		case('A'):
		State=1;
		break;
		
		case('B'):
		State=0;
		break;
	}
		
	if (State ==1)
	{
		
	if ((fl>12) && (fr>12)) {
		
		speedl = -300;
		speedr = -300;

		} else if ((fll<10) && (frr>12)) {
		
		speedl = -600;
		
		

		} else if ((fll>12) && (frr<10)) {
		
		speedr = -600;
		
		}else if ((fll<12) && (frr<12))
		{
			speedl=150;
			speedr=150;
			delay(400);
			speedl=0;
			speedr=0;
		}
		
		}
		if (State==0)
		{
			speedl=0;
			speedr=0;
		}
		
		motpwm_setLeft(speedl);
		motpwm_setRight(speedr);
		
}

