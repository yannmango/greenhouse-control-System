#include <avr/io.h>
#include <avr/delay.h>
#include "motor.h"
#include "common.h"



/**************************步进电机***************************/
//PORTD的 4,5,6,7
uchar ni[]={0x30,0x60,0xc0,0x90};   //电机逆时针方向转
uchar shun[]={0x30,0x90,0xC0,0x60};   //电机顺时针方向转


void motor_shun(uchar n)
{	  
        unsigned char i=0,j = 128;
 	 	DDRD = 0XF0;
		PORTD = 0XFF;
		//顺时针方向转
		while(n!='0')
		{
		
		while(j--)
		{
			if(i>=4) i%=4;
			PORTD = shun[i];
			i++;
			_delay_ms(3);
			
			if(j==0)return;
		}
	    }
		return;
	  
}


void motor_ni(uchar n)   
{	
        unsigned char i=0,j = 128;
 	 	DDRD = 0XF0;
		PORTD = 0XFF;
		//顺时针方向转
		while(n!='0')
		{
		while(j--)
		{
			if(i>=4) i%=4;
			PORTD = ni[i];
			i++;
			_delay_ms(3);

			if(j==0)return;
		}
		}
		return;
}

