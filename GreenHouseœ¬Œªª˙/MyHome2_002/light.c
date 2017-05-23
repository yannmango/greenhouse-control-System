
#include <avr/io.h>
#include <util/delay.h>
#include "light.h"
#include "common.h"



/************************光敏传感器***************************/

uchar get_light(void)//获取光敏传感器数据
{
 	CLRBIT(DDRB,LIGHT_BUS);
	_delay_ms(30);
	
	if(GETBIT(PINB, LIGHT_BUS))
	   return 0x11;
	else 
	  return 0xFF;
}
