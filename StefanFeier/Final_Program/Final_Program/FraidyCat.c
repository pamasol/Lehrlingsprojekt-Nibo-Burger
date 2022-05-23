void FraidyCat() {
	
	int FC_FC_lspeed=FC_left_speed;
	int FC_FC_rspeed=FC_right_speed;
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	char key = key_get_char();
	
	switch (key) {
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
			FC_lspeed=left_speed;
			FC_rspeed=right_speed;
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
		
		switch (i)  {
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
			motpwm_setLeft(left_speed);
			motpwm_setRight(right_speed);
						
			break;
			}
}
	if(FC_instruct==0) {
		motpwm_setLeft(0);
		motpwm_setRight(0);
	}
}