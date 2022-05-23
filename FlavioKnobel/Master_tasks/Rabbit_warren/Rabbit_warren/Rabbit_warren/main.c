
// only with slow gears

#include "niboburger/robomain.h"
#include "maroon.h"
int MotSpeedL;
int MotSpeedR;
int State;

uint8_t maroon_mode;

void setup(){
motpwm_init();
analog_init();
surface_readPersistent();
maroon_setup();
surface_init();
analog_setExtToggleMode(ANALOG_BCL, 1);
analog_setExtToggleMode(ANALOG_BCR, 1);
}

void maroon_setup() {
usart_setbaudrate(38400);
usart_enable();
delay(500);
}
void loop() {

uint8_t bcl = min(127, surface_get(SURFACE_CL)/8); // Line is left
uint8_t bc  = min(127, surface_get(SURFACE_C)/8);  // Line is in the center
uint8_t bcr = min(127, surface_get(SURFACE_CR)/8); // Line is right
	
char key = key_get_char();
	
switch (key) 
{
	case 'A':
	State=1;
	break;
		
	case 'B':
	State=0;
	break;
}

if (State==1)
{
	if (bc<80)
	{
	MotSpeedL=-470;
	MotSpeedR=-470;
	}
	if (bcr>=40&&bcr<=50)
	{
	MotSpeedR=-480;
	MotSpeedL=-350;
	}
	
	if (bcr>50)
	{
	MotSpeedR=-540;
	MotSpeedL=560;
	}
	
	if (bcr>67)
	{
	MotSpeedR=-980;
	MotSpeedL=1024;
	}
	
	if (bcl>=40&&bcl<50)
	{
	MotSpeedL=-480;
	MotSpeedR=-350;
	}
	
	if (bcl>50)
	{
	MotSpeedL=-540;
	MotSpeedR=560;
	}
	
	if (bcl>67)
	{
	MotSpeedL=-980;
	MotSpeedR=1024;
	}
	
	if (bc>70)
	{
	MotSpeedL=0;
	MotSpeedR=0;
	}
	
}

if(State==0)
{
MotSpeedL=0;
MotSpeedR=0;
}

motpwm_setLeft(MotSpeedL);
motpwm_setRight(MotSpeedR);
}