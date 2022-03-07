#include <niboburger/robomain.h>

int Speedl;
int Speedr;
int Status=0;
int Led=0;
int V1=30;
int V2=100;

void setup() {
	analog_init();
	led_init();
	motpwm_init();
}

void loop() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations with the same analog
	values is unrewarding.
	*/
	analog_wait_update();
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);

	// Checking left area of robot
	int l = max(fll, fl);
	// Checking right area of robot
	int r = max(frr, fr);
	
	char key = key_get_char();

switch (key) {
	case 'A':
	Status=1;
	break;
	 
	case 'B':
	Status=0;
	break; 
	
	case 'C':
	Led=1;
	break;
	}
	
if(Status==0){
	Speedl=0;
	Speedr=0;
}

if (Status==1){

	if ((l<10) && (r<10)) {
		/*
		No obstacles, move with 80% of full speed.
		*/
		Speedl=-350;
		Speedr=-380;

		} else if ((l<10) && (r>10)) {
		/*
		Obstacles in right area, turn left.
		*/
		Speedl=300;
		Speedr=-500;

		} else if ((l>10) && (r<10)) {
		/*
		Obstacles in left area, turn right.
		*/
		Speedl=-500;
		Speedr=300;
		
		} else if ((l>10) && (r>10)){
		/*
		Obstacles, turn.
		*/
		Speedl=-300;
		Speedr=-330;
		delay(400);
		Speedl=-300;
		Speedr=330;	
}
}
if (Led==1) {
	led_set(2,1);
	delay(V1);
	led_set(2,0);
	delay(V1);
	led_set(2,1);
	delay(V1);
	led_set(2,0);
	delay(V1);
	led_set(2,1);
	delay(V1);
	led_set(2,0);
	delay(V2);
	led_set(3,1);
	delay(V1);
	led_set(3,0);
	delay(V1);
	led_set(3,1);
	delay(V1);
	led_set(3,0);
	delay(V1);
	led_set(3,1);
	delay(V1);
	led_set(3,0);
	delay(V2);
}
motpwm_setLeft(Speedl);
motpwm_setRight(Speedr);
}