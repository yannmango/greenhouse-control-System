


#include <avr/io.h>
#include <util/delay.h>
#include "common.h"
#include "led.h"



void led_test(uchar led_count)
{
	//����PA0Ϊ���ģʽ
	DDRA=0xFF;

    //PORTA = 0x00;
    //_delay_ms(10);
   // PORTA ^=1<<led_count;
    PORTA  = ~PORTA;

}














