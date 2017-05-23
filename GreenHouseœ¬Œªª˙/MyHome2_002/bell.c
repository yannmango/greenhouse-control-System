#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "bell.h"
#include "common.h"


/***************************蜂鸣器*****************************/

void 	bell(uint n)//蜂鸣器连续发声
{
    int i;
	SETBIT(DDRD, BELL_BUS); //设置为输出模式
    cli();
	while(n--) 
	{
        _delay_ms(200);
		for(i=0;i<10;i++)
		{  
		   _delay_ms(100);
		   SETBIT(PORTD, BELL_BUS);
		   _delay_ms(100);
		   CLRBIT(PORTD, BELL_BUS);
		 }

		//_delay_ms(300);
	}
	CLRBIT(PORTD, BELL_BUS); //关闭蜂鸣器
	sei();
}

//回复响应

