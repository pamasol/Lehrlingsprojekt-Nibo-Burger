void RoundTrip() {

	int RW_lspeed=left_speed;
	int RW_rspeed=right_speed;
	
	char key = key_get_char();
	
	switch (key) {
		case 'A':
		RW_instruct=1;
		break;	
		case 'B':
		RW_instruct=0;
	}
		
	
	
	if(RW_instruct==1)	{
		

		if(odometry_getLeft(0)>odometry_getRight(0))	{
			RW_rspeed=RW_rspeed+100;
		}
		if(odometry_getRight(0)>odometry_getLeft(0))	{
			RW_lspeed=RW_lspeed+100;
		}
		if(odometry_getLeft(0)==odometry_getRight(0))	{
			RW_lspeed=left_speed;
			RW_rspeed=right_speed;
		}	

	
		
		switch (i)  {
			case 1:
			motpwm_setLeft(RW_lspeed);
			motpwm_setRight(RW_rspeed);
			if(odometry_getLeft(0)>=1150)	{
			odometry_getLeft(1);
			odometry_getRight(1);
			delay(100);
			RW_i=2;
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
			RW_i=3;
			
			break;
			
			case 3:
				motpwm_setLeft(RW_lspeed);
				motpwm_setRight(RW_rspeed);
				if(odometry_getLeft(0)>=1150)	{
				motpwm_setLeft(0);
				motpwm_setRight(0);
				delay(100);
				}
			
			break;
			}
}
	if(RW_instruct==0) {
		RW_i=1;
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