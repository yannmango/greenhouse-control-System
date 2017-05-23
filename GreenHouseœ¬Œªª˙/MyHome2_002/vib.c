
#include <avr/io.h>
#include <avr/delay.h>
#include "vib.h"
#include "common.h"


/************************�𶯴�����***************************/


uchar get_vib(void) //��ȡ�𶯴���������
{
    CLRBIT(DDRD, VIB_BUS);
	_delay_ms(30);

	if(GETBIT(PIND, VIB_BUS))
	   return 0xFF;
	else 
	   return 0x11;
}
