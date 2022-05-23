
#include <niboburger/robomain.h>
#include "maroon.h"
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>

uint8_t maroon_mode;

volatile int prog_select = 0;
int select = 0;
int sel_1 = 0;
int sel_2 = 0;
int sel_3 = 0;
int sel_4 = 0;
int sel_5 = 0;

int bat_volt = 0;
int bat_volt2 = 0;
int bat_volt3 = 0;
char bat[8];
char bat2[8];

int led;

long prog1 = 0;
long prog2 = 0;
long prog3 = 0;
long prog4 = 0;
long prog5 = 0;



int RT_instruct=0;
int RT_i=1;
const int RT_left_speed=600; //Max 900!
const int RT_right_speed=600; //Max 900!

int FC_instruct=0;
int FC_i=1;
const int FC_left_speed=600; //Max 900!
const int FC_right_speed=600; //Max 900!

int FM_instruct=0;
int FM_i=1;
const int FM_left_speed=600; //Max 900!
const int FM_right_speed=600; //Max 900!

int RW_instruct=0;
int RW_i=0;
int RW_start=0;
const int RW_left_speed=400; //Max 900!
const int RW_right_speed=400; //Max 900!

void maroon_setup() {
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

void setup() {
	
	led_init();
	analog_init();
	surface_readPersistent();
	maroon_setup();
	motpwm_init();
	odometry_init();
	
	int bat_volt = nibo_getMillivolt();

	
	char bat_key;
	int button1 = 0;

	bat_key = key_get_char();
	
	
	switch (bat_key) {
		case 'A':
		button1=1;
		led_set(1,1);
		delay(100);
		led_set(1,0);
		break;
	}

	if(button1==1)	{
	
		if(bat_volt>=5000)	{
			usart_write("100%   ");
		}
		else if(bat_volt>=4874)	{
			usart_write("95%   ");
		}
		else if(bat_volt>=4752)	{
			usart_write("90%   ");
		}
		else if(bat_volt>=4630)	{
			usart_write("85%   ");
		}
		else if(bat_volt>=4508)	{
			usart_write("80%   ");
		}
		else if(bat_volt>=4386)	{
			usart_write("75%   ");
		}
		else if(bat_volt>=4264)	{
			usart_write("70%   ");
		}
		else if(bat_volt>=4142)	{
			usart_write("65%   ");
		}
		else if(bat_volt>=4020)	{
			usart_write("60%   ");
		}
		else if(bat_volt>=3898)	{
			usart_write("55%   ");
		}
		else if(bat_volt>=3776)	{
			usart_write("50%   ");
		}
		else if(bat_volt>=3645)	{
			usart_write("45%   ");
		}
		else if(bat_volt>=3532)	{
			usart_write("40%   ");
		}
		else if(bat_volt>=3410)	{
			usart_write("35%   ");
			button1=0;
			delay(6000);
		}
		else if(bat_volt>=3288)	{
			usart_write("30%   ");
			button1=0;
			delay(6000);
		}
		else if(bat_volt>=3166)	{
			usart_write("25%   ");
			button1=0;
			delay(6000);
		}
		else if(bat_volt>=3044)	{
			usart_write("20%   ");
			button1=0;
			delay(6000);
		}
		else if(bat_volt>=2922)	{
			usart_write("15%   ");
			button1=0;
			delay(6000);
		}
		else if(bat_volt>=2800)	{
			usart_write("10%   ");
			button1=0;
			delay(6000);
		}
		
		else if(bat_volt>=2700)	{
			usart_write("5%   ");
			button1=0;
			delay(6000);
		}
		else
		{
			usart_write("0%   ");
			button1=0;
			delay(6000);
		}
	}
	
	
	delay(500);
	if(button1==1)	{
		delay(4500);
		usart_write(MAROON_IMM_CLEAR());
		usart_write("On   ");
		
	while(led<=5)	{
		led_setall(1,0,0,0);
		delay(80);
		led_setall(0,1,0,0);
		delay(80);
		led_setall(0,0,1,0);
		delay(80);
		led_setall(0,0,0,1);
		delay(80);
		led=led+1;
	}
		
		led_setall(1,0,0,0);
		delay(80);
		led_setall(1,1,0,0);
		delay(80);
		led_setall(1,1,1,0);
		delay(80);
		led_setall(1,1,1,1);
		delay(80);
		
		delay(1000);
		usart_write(MAROON_IMM_CLEAR());
		led_setall(0,0,0,0);
	}
	else
	{
		usart_write("Off   ");
		delay(4000);
		usart_write(MAROON_IMM_CLEAR());
		while(true){
			
		}
	}
}


void loop() {
	
	



	
if(usart_rxempty()==false)	{
	
	int key;

	key = usart_getchar();
	
	switch (key) {
		case 65:
		delay(200);
		prog_select=prog_select+1;

		break;
		case 97:
		break;
		case 66:
		select = 1;
		led_set(3,1);
		break;
		case 98:
		//led_set(2,0);
		break;
		}
		
		
		
	}
	
	switch(prog_select)	{
		case 1:
		if(prog1==0)	{
		usart_write(MAROON_IMM_CLEAR());
		led_setall(1,0,0,0);
		usart_write("Round Trip   ");
		prog1=1;
		}
		
		if(prog1!=0) {
		prog1=prog1+1;	
		delay(500);	
		}
		
		if(prog1==14)	{
			prog1=0;
		}
		break;
		
		case 2:
		if(prog2==0)	{
		usart_write(MAROON_IMM_CLEAR());
		led_setall(0,1,0,0);
		usart_write("Fraidy Cat   ");
		prog2=1;
		}
		
		if(prog2!=0) {
			prog2=prog2+1;
			delay(500);
		}
		
		if(prog2==14)	{
			prog2=0;
		}
		break;
		
		case 3:
		if(prog3==0)	{
			usart_write(MAROON_IMM_CLEAR());
			led_setall(0,0,1,0);
			usart_write("Follow Me   ");
			prog3=1;
		}
		
		if(prog3!=0) {
			prog3=prog3+1;
			delay(500);
		}
		
		if(prog3==13)	{
			prog3=0;
		}
		break;
		
		case 4:
		if(prog4==0)	{
			usart_write(MAROON_IMM_CLEAR());
			led_setall(0,0,0,1);
			usart_write("Color Detection   ");
			prog4=1;
		}
		
		if(prog4!=0) {
			prog4=prog4+1;
			delay(500);
		}
		
		if(prog4==20)	{
			prog4=0;
		}
		break;
		
		case 5:
		if(prog5==0)	{
			usart_write(MAROON_IMM_CLEAR());
			led_setall(1,0,0,1);
			usart_write("Rabbit Warren   ");
			prog5=1;
		}
		
		if(prog5!=0) {
			prog5=prog5+1;
			delay(500);
		}
		
		if(prog5==18)	{
			prog5=0;
		}
		break;
		
		default:
		led_setall(0,0,0,0);
		prog_select=1;
		prog1=0;
		prog2=0;
		prog3=0;
		prog4=0;
		prog5=0;
		delay(200);
		usart_write(MAROON_IMM_CLEAR());
		break;
	}







//RoundTrip
	while(prog_select==1 && select==1)	{

	
		int RT_lspeed=RT_left_speed;
		int RT_rspeed=RT_right_speed;
		
		char RT_key = key_get_char();
		
		switch (RT_key) {
			case 'A':
			RT_instruct=1;
			break;
			case 'B':
			RT_instruct=0;
		}
		
		
		
		if(RT_instruct==1)	{
			

			if(odometry_getLeft(0)>odometry_getRight(0))	{
				RT_rspeed=RT_rspeed+100;
			}
			if(odometry_getRight(0)>odometry_getLeft(0))	{
				RT_lspeed=RT_lspeed+100;
			}
			if(odometry_getLeft(0)==odometry_getRight(0))	{
				RT_lspeed=RT_left_speed;
				RT_rspeed=RT_right_speed;
			}

			
			
			switch (RT_i)  {
				case 1:
				motpwm_setLeft(RT_lspeed);
				motpwm_setRight(RT_rspeed);
				if(odometry_getLeft(0)>=1150)	{
					odometry_getLeft(1);
					odometry_getRight(1);
					delay(100);
					RT_i=2;
				}
				break;
				
				case 2:
				while(odometry_getLeft(0)<142)	{
					motpwm_setLeft(400);
					motpwm_setRight(-400);
				}
				
				motpwm_setLeft(0);
				motpwm_setRight(0);
				odometry_getLeft(1);
				odometry_getRight(1);
				delay(100);
				RT_i=3;
				
				break;
				
				case 3:
				motpwm_setLeft(RT_lspeed);
				motpwm_setRight(RT_rspeed);
				if(odometry_getLeft(0)>=1150)	{
					motpwm_setLeft(0);
					motpwm_setRight(0);
					delay(100);
				}
				
				break;
			}
		}
		if(RT_instruct==0) {
			RT_i=1;
			motpwm_setLeft(0);
			motpwm_setRight(0);
			odometry_getLeft(1);
			odometry_getRight(1);
		}
		led_set(1, odometry_getLeft(0)>300);
		led_set(2, odometry_getLeft(0)>600);
		
		led_set(3, odometry_getRight(0)>900);
		led_set(4, odometry_getRight(0)>1100);
		
	}




//FraidyCat() {
while(prog_select==2 && select==1)	{	
	int FC_lspeed=FC_left_speed;
	int FC_rspeed=FC_right_speed;
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	char FC_key = key_get_char();
	
	switch (FC_key) {
		case 'A':
		FC_instruct=1;
		break;
		case 'B':
		FC_instruct=0;
	}
	
	
	
	if(FC_instruct==1)	{
		
		if(odometry_getLeft(0)>odometry_getRight(0))	{
			FC_rspeed=FC_rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			FC_lspeed=FC_lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			FC_lspeed=FC_left_speed;
			FC_rspeed=FC_right_speed;
		}
		
		
		if (fll>30)	{
			led_set(1,1);
			FC_i=1;
		}
		else
		{
			
		}
		
		if (fl>30)	{
			led_set(2,1);
			FC_i=2;
		}
		else
		{
			
		}
		
		if (fr>30)	{
			led_set(3,1);
			FC_i=3;
		}
		else
		{
			
		}
		
		if (frr>30)	{
			led_set(4,1);
			FC_i=4;
		}
		else
		{
			
		}
		
		if ((fll>30) && (fl>30) && (fr>30) && (frr>30))	{
			FC_i=5;
		}
		else
		{
			
		}
		
		
		if ((fll>30) && (frr>30) && (fl<30) && (fr<30))	{
			FC_i=6;
		}
		else
		{
			
		}
		
		if ((fl>30) && (fr>30) && (fll<30) && (frr<30))	{
			FC_i=6;
		}
		else
		{
			
		}
		
		if (fll<20)	{
			led_set(1,0);
		}
		else
		{
			
		}
		
		if (fl<20)	{
			led_set(2,0);
		}
		else
		{
			
		}
		
		if (fr<20)	{
			led_set(3,0);
		}
		else
		{
			
		}
		
		if (frr<20)	{
			led_set(4,0);
		}
		else
		{
			
		}
		
		switch (FC_i)  {
			case 1:	//obstacle on the far left side
			motpwm_setLeft(400);
			motpwm_setRight(-400);
			delay(600);
			FC_i=0;
			
			break;
			
			
			case 2:	//obstacle on the left side
			motpwm_setLeft(400);
			motpwm_setRight(-400);
			delay(600);
			FC_i=0;
			
			break;
			
			
			case 3:	//obstacle on the right side
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(600);
			FC_i=0;
			
			break;
			
			
			case 4:	//obstacle on the far right side
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(600);
			FC_i=0;
			
			break;
			
			
			case 5:	//corner
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(700);
			FC_i=0;
			
			break;
			
			
			case 6:	//corner
			motpwm_setLeft(-400);
			motpwm_setRight(400);
			delay(1500);
			FC_i=0;
			
			break;
			
			
			default:	//no obstacle
			motpwm_setLeft(FC_left_speed);
			motpwm_setRight(FC_right_speed);
			
			break;
		}
	}
	if(FC_instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
}




//FollowMe() {
while(prog_select==3 && select==1)	{		
	int FM_lspeed=FM_left_speed;
	int FM_rspeed=FM_right_speed;
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	char FM_key = key_get_char();
	
	switch (FM_key) {
		case 'A':
		FM_instruct=1;
		break;
		case 'B':
		FM_instruct=0;
	}
	
	
	
	if(FM_instruct==1)	{
		
		if(odometry_getLeft(0)>odometry_getRight(0))	{
			FM_rspeed=FM_rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			FM_lspeed=FM_lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			FM_lspeed=FM_left_speed;
			FM_rspeed=FM_right_speed;
		}
		
		
		if (fll>30)	{
			led_set(1,1);
			FM_i=1;
		}
		else
		{
			
		}
		
		if (fl>30)	{
			led_set(2,1);
			FM_i=2;
		}
		else
		{
			
		}
		
		if (fr>30)	{
			led_set(3,1);
			FM_i=3;
		}
		else
		{
			
		}
		
		if (frr>30)	{
			led_set(4,1);
			FM_i=4;
		}
		else
		{
			
		}
		
		if ((fll>30) && (fl>30) && (fr>30) && (frr>30))	{
			FM_i=5;
		}
		else
		{
			
		}
		
		
		if ((fll>30) && (frr<30) && (fl>30) && (fr>30))	{
			FM_i=2;
		}
		else
		{
			
		}
		
		if ((fll<30) && (frr>30) && (fl>30) && (fr>30))	{
			FM_i=3;
		}
		else
		{
			
		}
		
		if ((fll<30) && (frr<30) && (fl>30) && (fr>30))	{
			FM_i=5;
		}
		else
		{
			
		}
		
		if ((fll>30) && (frr>30) && (fl<30) && (fr<30))	{
			FM_i=5;
		}
		else
		{
			
		}
		
		if (fll<20)	{
			led_set(1,0);
		}
		else
		{
			
		}
		
		if (fl<20)	{
			led_set(2,0);
		}
		else
		{
			
		}
		
		if (fr<20)	{
			led_set(3,0);
		}
		else
		{
			
		}
		
		if (frr<20)	{
			led_set(4,0);
		}
		else
		{
			
		}
		
		switch (FM_i)  {
			case 1:	//hand on the far left side
			motpwm_setLeft(FM_lspeed/8);
			motpwm_setRight(FM_rspeed);
			delay(600);
			FM_i=0;
			
			break;
			
			
			case 2:	//hand on the left side
			motpwm_setLeft(FM_lspeed/2);
			motpwm_setRight(FM_rspeed);
			delay(50);
			FM_i=0;
			
			break;
			
			
			case 3:	//hand on the right side
			motpwm_setLeft(FM_lspeed);
			motpwm_setRight(FM_rspeed/2);
			delay(50);
			FM_i=0;
			
			break;
			
			
			case 4:	//hand on the far right side
			motpwm_setLeft(FM_lspeed);
			motpwm_setRight(FM_rspeed/8);
			delay(50);
			FM_i=0;
			
			break;
			
			
			case 5:	//hand in front
			motpwm_setLeft(FM_lspeed);
			motpwm_setRight(FM_rspeed);
			delay(50);
			FM_i=0;
			
			break;
			
			
			default:	//no hand
			motpwm_setLeft(0);
			motpwm_setRight(0);
			
			break;
		}
	}
	if(FM_instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
}




//ColorDetection() {
while(prog_select==4 && select==1)	{		
	//used for testing
	/*
	char red_val[16];
	char green_val[16];
	char blue_val[16];
	char yellow_val[16];
	char white_val[16];
	char black_val[16];
	*/
	unsigned long int col = surface_getColorRGB();
	
	uint16_t diff_red = color_diff_rgb(col, COLOR_RGB_CAL_RED);
	uint16_t diff_green = color_diff_rgb(col, COLOR_RGB_CAL_GREEN);
	uint16_t diff_blue = color_diff_rgb(col, COLOR_RGB_CAL_BLUE);
	uint16_t diff_yellow = color_diff_rgb(col, COLOR_RGB_CAL_YELLOW);
	uint16_t diff_white = color_diff_rgb(col, COLOR_RGB_CAL_WHITE);
	//uint16_t diff_black = color_diff_rgb(col, COLOR_RGB_CAL_BLACK); //(not used)
	
	//display sensor values on maroon shield (testing)
	/*
	sprintf(red_val, "%u", diff_red);
	sprintf(green_val, "%u", diff_green);
	sprintf(blue_val, "%u", diff_blue);
	sprintf(yellow_val, "%u", diff_yellow);
	sprintf(white_val, "%u", diff_white);
	sprintf(black_val, "%u", diff_black);
	
	usart_write(MAROON_IMM_CLEAR());
	usart_write(red_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(green_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(blue_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(yellow_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(white_val);
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	usart_write(black_val);
	delay(5000);
	usart_write("F");
	delay(5000);
	usart_write(MAROON_IMM_CLEAR());
	delay(5000);
	*/
	
	//
	if(diff_red<4000)	{
		usart_write(MAROON_IMM_CLEAR());
		usart_write("RED");
		delay(5000);
	}
	
	if(diff_green<10000)	{
		usart_write(MAROON_IMM_CLEAR());
		usart_write("GREEN");
		delay(5000);
	}
	
	if(diff_blue<10000)	{
		usart_write(MAROON_IMM_CLEAR());
		usart_write("BLUE");
		delay(5000);
	}
	
	if(diff_yellow<5500)	{
		usart_write(MAROON_IMM_CLEAR());
		usart_write("YELLOW");
		delay(5000);
	}
	
	if(diff_white<6200)	{
		usart_write(MAROON_IMM_CLEAR());
		usart_write("WHITE");
		delay(5000);
	}

	if((diff_red>=32000) && (diff_green>=32000) && (diff_blue>14000) && (diff_yellow>=32000) && (diff_white>=32000))	{
		usart_write(MAROON_IMM_CLEAR());
		delay(500);
	}
	
}




if(prog_select==5 && select==1 && RW_start==0)	{	
//RabbitWarrenSetup() {
	analog_init();
	led_init();
	motpwm_init();
	odometry_init();
	surface_init();
	analog_setExtToggleMode(ANALOG_BCL, 1);
	analog_setExtToggleMode(ANALOG_BCR, 1);
	usart_setbaudrate(38400);
	usart_enable();
	delay(500);
}

while(prog_select==5 && select==1)	{
//RabbitWarren() {
	/*
	The loop is executed only if a value of the analog inputs changes.
	This makes sense since thousands of iterations with the same analog
	values is unrewarding.
	*/
	RW_start=1;
	analog_wait_update();
	
	int RW_lspeed=RW_left_speed;
	int RW_rspeed=RW_right_speed;
	
	char RW_key = key_get_char();
	
	switch (RW_key) {
		case 'A':
		RW_instruct=1;
		break;	
		case 'B':
		RW_instruct=0;
	}
		
	
	
	if(RW_instruct==1)	{
		
		//char bcr_val[8];
		
		uint8_t bcr = min(127, surface_get(SURFACE_CR)/8); // Green light on right transistor
		uint8_t bcl = min(127, surface_get(SURFACE_CL)/8); // Green light on left transistor
		//uint8_t bl  = min(127, surface_get(SURFACE_L) /8);
		uint8_t bc  = min(127, surface_get(SURFACE_C) /8);
		//uint8_t br  = min(127, surface_get(SURFACE_R) /8);
		
		
		//sprintf(bcr_val, "%u", bc);
		
		
		if(odometry_getLeft(0)>odometry_getRight(0))	{
			RW_rspeed=RW_rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			RW_lspeed=RW_lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			RW_lspeed=RW_left_speed;
			RW_rspeed=RW_right_speed;
		}
		
		
		if(bc<=65)	{
			led_set(2,1);
			led_set(3,1);
		}
		
		if(bcl<=90)	{
			led_set(1,1);
		}
		
		if(bcr<=65)	{
			led_set(4,1);
		}
		
		
		if(bc>65)	{
			led_set(2,0);
			led_set(3,0);
		}
		
		if(bcl>90)	{
			led_set(1,0);
		}
		
		if(bcr>65)	{
			led_set(4,0);
		}
		
		
		if((bc<=65) && (bcl<=90) && (bcr<=65))	{
			RW_i=1;
		}
		
		if((bc<=65) && (bcl<=90) && (bcr>=65))	{
			RW_i=2;
		}
		
		if((bc<=65) && (bcl>=90) && (bcr<=65))	{
			RW_i=3;
		}
		
		if((bc>=65) && (bcl<=90) && (bcr>=65))	{
			RW_i=4;
		}
		
		if((bc>=65) && (bcl>=90) && (bcr<=65))	{
			RW_i=5;
		}
		
		if((bc>=65) && (bcl>=90) && (bcr>=65))	{
			RW_i=6;
		}
		
		
		if(RW_i==1)	{
			motpwm_setLeft(RW_left_speed);
			motpwm_setRight(RW_right_speed);
			delay(10);
		}
		
		if(RW_i==2)	{
			motpwm_setLeft(RW_left_speed-1200);
			motpwm_setRight(RW_right_speed+600);
			delay(10);
		}
		
		if(RW_i==3)	{
			motpwm_setLeft(RW_left_speed+600);
			motpwm_setRight(RW_right_speed-1200);
			delay(10);
		}
		
		if(RW_i==4)	{
			motpwm_setLeft(-800);
			motpwm_setRight(RW_right_speed+1000);
			delay(10);
		}
		
		if(RW_i==5)	{
			motpwm_setLeft(RW_left_speed+1000);
			motpwm_setRight(-800);
			delay(10);
		}
		
		if(RW_i==6)	{
			motpwm_setLeft(0);
			motpwm_setRight(0);
			delay(80);
		}
		
	}
	if(RW_instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
		RW_i=0;
	}

	


}
	
}