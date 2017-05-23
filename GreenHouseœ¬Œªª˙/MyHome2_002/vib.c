
#include <avr/io.h>
#include <avr/delay.h>
#include "vib.h"
#include "common.h"


/************************震动传感器***************************/


uchar get_vib(void) //获取震动传感器数据
{
    CLRBIT(DDRD, VIB_BUS);
	_delay_ms(30);

	if(GETBIT(PIND, VIB_BUS))
	   return 0xFF;
	else 
	   return 0x11;
}
