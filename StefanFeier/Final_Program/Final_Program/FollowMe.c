void FollowMe() {
	
	int FM_lspeed=FM_left_speed;
	int FM_rspeed=FM_right_speed;
	
	int fll = analog_getValueExt(ANALOG_FLL, 2);
	int fl = analog_getValueExt(ANALOG_FL, 2);
	int fr = analog_getValueExt(ANALOG_FR, 2);
	int frr = analog_getValueExt(ANALOG_FRR, 2);
	
	char key = key_get_char();
	
	switch (key) {
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
			FM_lspeed=left_speed;
			FM_rspeed=right_speed;
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
		
		switch (i)  {
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