#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom162.h>
#include "myDHT11.h"


#define BIT(bit) (1<<(bit))


#define DATA_OUTPUT               DDRB|=BIT(PB0)
#define DATA_INPUT                DDRB&=~BIT(PB0)
#define DATA_H                    PORTB|=BIT(PB0)
#define DATA_L                    PORTB&=~BIT(PB0)
#define DATA_BUS                   ((PINB & BIT(PB0)) >> PB0)

#define delay_30_ms()                delay_ms(30)
#define delay_30_us()                delay_us(30)
#define delay_10_us()                delay_us(10)

uint16 temperature; //temperature�洢�¶�,��8λΪ�¶���������,��8λ����Ϊ0
uint16 humidity; 	//humidity�洢ʪ��,��8λΪʪ����������,��8λ����Ϊ0
extern uint8	hum_h = 0;
uint8	hum_l;
uint8	tem_h;
uint8	tem_l;

//��������
void 	delay_us(unsigned char del);
void 	delay_ms(long int s);
unsigned char get_DHT11_value(void);

 

/********************************************************************
 * �������ܣ�΢����ʱ�ӳ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺����n����ʾ��ʱʱ��ĳ���
 * ��    �أ�
 * *****************************************************************/
void delay_us(unsigned char us)
{
	while(us--);
}

/********************************************************************
 * �������ܣ�������ʱ�ӳ���
 * ע�����
 * ��ʾ˵����
 * ��    �룺����n����ʾ��ʱʱ��ĳ���
 * ��    �أ�
 * *****************************************************************/
void delay_ms(long int ms)
{
	int t;
	while(ms--) {
		for(t=1000;t>0;t--);
	}
}






unsigned char get_DHT11_value(void)
{        
	uint8 bit;
	uint8 p=0,time_count;         
	hum_h = 0x00;
	tem_h = 0x00;

	bit=0x80;

	DATA_L;		//PORTB &= ~BIT(PB0)
	DATA_OUTPUT;
	//delay_30_ms();
	_delay_ms(30);

	DATA_H;
	DATA_INPUT;
	_delay_us(60);
	//delay_30_us();
	//delay_30_us();

	//��Ӧ���ź�,�˳�
	if(DATA_BUS!=0) return 0;         
	//�ȴ���ƽ���,����200us��ʱ����
	time_count=0;
	while(DATA_BUS==0) {        
		//delay_10_us();
		_delay_us(10);
		if(++time_count==20)return 0;
	}         
	//�ȴ���ƽ���,����150us��ʱ����
	time_count=0;
	while(DATA_BUS==1) {        
		//delay_10_us();
		_delay_us(10);
		if(++time_count==15)return 0;
	}

	//���ݿ�ʼ
	while(p!=4) {        
		//�͵�ƽ���50us
		//�ȴ���ƽ���,����100us��ʱ����
		time_count=0;
		while(DATA_BUS==0) {        
			//delay_10_us();
			_delay_us(10);
			if(++time_count==10) return 0;
		}

		time_count=0;
		do {         
			//delay_10_us();
			_delay_us(10);
			time_count++;
			if(time_count==10) return 0;
		}while(DATA_BUS==1);

		//���ߵ�ƽ����50us,˵������λΪ1,����Ϊ0
		if(time_count>=5) {        
			if(p==0) hum_h |=bit;
			if(p==1) hum_l |=bit;
			if(p==2) tem_h |=bit;
			if(p==3) tem_l |=bit;
		}

		bit>>=1;
		//1�ֽڽ���
		if(bit==0x00) {        
			p++;
			bit=0x80;
		}
	}

	return 1;
}
 

