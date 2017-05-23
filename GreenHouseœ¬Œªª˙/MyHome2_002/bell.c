#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "bell.h"
#include "common.h"


/***************************������*****************************/

void 	bell(uint n)//��������������
{
    int i;
	SETBIT(DDRD, BELL_BUS); //����Ϊ���ģʽ
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
	CLRBIT(PORTD, BELL_BUS); //�رշ�����
	sei();
}

//�ظ���Ӧ

