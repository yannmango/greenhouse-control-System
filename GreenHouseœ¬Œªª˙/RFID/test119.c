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
	while (!( UCSR0A & (1<<UDRE0)));//����������һ׼������
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
	UCSR0A=0x00;			//������ģʽ
	UCSR0B=0x18;			//�����жϺͷ����ж�ʹ��
	UCSR0C=0x86;			//�첽ͨ����У�飬8λ����λ��1λֹͣλ
	UBRR0H=baud_h;
	UBRR0L=baud_l;
}


int main(void)
{ 

/********************************************************************
 * ���RFID��Ƭ�ı��
 * 
 * ע�⣺init_rc522();��uint8 buf[4];���������һ��Ҫ��main()������д��
 *       getRFID(buf)��ı�buf*��ָ���ڴ��е����ݣ��������д�����У�
 *       ��ȡ��ʱ����Ҫ��buf[0],buf[1],buf[2],buf[3]ȡ��
 *       ������������һֱ��RFID������Ϣ��ʹ��ʱ��ͨ�����������
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
