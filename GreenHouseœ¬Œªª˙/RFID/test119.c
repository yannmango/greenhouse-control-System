//#include "common.h"
//#include "sensor.h"
//#include "phenomenon.h"
//#include "key.h"
//#include "motor.h"
#include "rc522.h"
#include "common.h"


#define baud 9600
#define baud_setting (unsigned int)((unsigned long)F_CPU/(16*(unsigned long)baud)-1)
#define baud_h	((uint8)(baud_setting>>8))
#define baud_l  ((uint8)(baud_setting))


void putch(uint8 data)
{
	while (!( UCSR0A & (1<<UDRE0)));//表明发送器一准备就绪
	UDR0 = data;
}


void	puts(uint8* s)
{
	while(*s) {
		putch(*s);
		s++;
	}
}

uint8	getch(void)
{
	while(!(UCSR0A&(1<<RXC0)));
	return UDR0;
}


void USART_Init(void)
{
	UCSR0A=0x00;			//单倍速模式
	UCSR0B=0x18;			//接收中断和发送中断使能
	UCSR0C=0x86;			//异步通信无校验，8位数据位和1位停止位
	UBRR0H=baud_h;
	UBRR0L=baud_l;
}


int main(void)
{ 

/********************************************************************
 * 获得RFID卡片的编号
 * 
 * 注意：init_rc522();和uint8 buf[4];这两个语句一定要在main()函数中写，
 *       getRFID(buf)会改变buf*所指向内存中的数据，将卡编号写入其中，
 *       读取的时候，需要将buf[0],buf[1],buf[2],buf[3]取出
 *       这个驱动程序会一直读RFID卡的信息，使用时可通过软件来控制
 *******************************************************************/


	USART_Init();

	init_rc522();

	uint8 buf[6];

    uint8 cmd;



	while(1){


	   cmd = getch();

       if( cmd == 0x30){
	
	      while(1)
	      {
		       if(getRFID(buf))
		       {

			
		        	puts(buf);
			 
		        	_delay_ms(50);

                    cmd = getch();

					if(cmd == 0x30)
					{
					    continue;
					}
					else
					{
					    break;
					}   

				
	
	        	}

          }

     	}
     	else
	    {
	       continue;
    	}
	

	}



	return 0;
}
