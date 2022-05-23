#include "niboburger/robomain.h"

void setup()
{
analog_init();
motpwm_init();
}

void loop()
{
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	int l = max(fll, fl);
	int r = max(frr, fr);
	int Speedl;
	int Speedr;
	
	if ((l<20) && (r<20)) {
		delay(10);
		Speedl=0;
		Speedr=0;
		} 
		
		else if ((l<15) && (r>15)) {
		delay(10);
		Speedl=-650;
		Speedr=400;
		} 
		
		else if ((l>15) && (r<15)) {
		delay(10);
		Speedl=400;
		Speedr=-650;
		} 
		
		else if ((l>10) && (r>10)){
		delay(10);
		Speedl=-650;
		Speedr=-650;
}
motpwm_setLeft(Speedl);
motpwm_setRight(Speedr);
	
}