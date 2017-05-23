
#include <avr/io.h>
#include <avr/delay.h>
#include "infra.h"
#include "common.h"


unsigned char get_inf(void)//获取红外传感器数据
{
 	
	CLRBIT(DDRB, INF_BUS);
	_delay_ms(30);

	if(GETBIT(PINB, INF_BUS))
	   return 0xFF;
	else 
	   return 0x11;
}
