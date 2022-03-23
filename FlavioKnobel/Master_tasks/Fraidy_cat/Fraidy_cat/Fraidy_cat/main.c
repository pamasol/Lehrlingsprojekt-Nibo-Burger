#include <niboburger/robomain.h>
#include "maroon.h"

int Speedl;
int Speedr;
int Status=0;
int Led=0;
int V1=30;
int V2=100;
int state = 2;

void maroon_vorwaerz() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("103070ffff703010")

	);
}
void maroon_left() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("18181818ff7e3c18")

	);
}
void maroon_right() {
	usart_write(
	MAROON_IMM_CLEAR()
	MAROON_BRIGHT(*)
	MAROON_LOAD()
	MAROON_GFX("183c7eff18181818")

	);
}

void setup() {
	analog_init();
	led_init();
	motpwm_init();
	usart_setbaudrate(38400);
	usart_enable();

}


void loop() {
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
		Speedl=-1000;
		Speedr=-1024;
		} 
		
		else if ((l<10) && (r>10)) {
		/*
		Obstacles in right area, turn left.
		*/
		Speedl=1000;
		Speedr=-1024;
		} 
		
		else if ((l>10) && (r<10)) {
		/*
		Obstacles in left area, turn right.
		*/
		Speedl=-1000;
		Speedr=1024;
		} 
		
		else if ((l>10) && (r>10)){
		/*
		Obstacles, turn.
		*/
		Speedl=-1000;
		Speedr=-1024;
		delay(400);
		Speedl=-1000;
		Speedr=1024;	
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

	if (state==2) {
	maroon_vorwaerz();
	state = 3;
}

	if ((l>10) && (r<10)) {
	state = 1;
	delay(10);
	maroon_right();
	delay(1000);
	state = 2;
}

	if ((l<10) && (r>10)) {
	state = 1;
	delay(10);
	maroon_left();
	delay(1000);
	state = 2;
} 
}
