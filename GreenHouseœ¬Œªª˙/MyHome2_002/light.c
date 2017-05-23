
#include <avr/io.h>
#include <util/delay.h>
#include "light.h"
#include "common.h"



/************************����������***************************/

uchar get_light(void)//��ȡ��������������
{
 	CLRBIT(DDRB,LIGHT_BUS);
	_delay_ms(30);
	
	if(GETBIT(PINB, LIGHT_BUS))
	   return 0x11;
	else 
	  return 0xFF;
}
