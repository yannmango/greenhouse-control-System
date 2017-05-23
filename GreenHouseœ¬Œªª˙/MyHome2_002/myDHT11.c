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

uint16 temperature; //temperature存储温度,高8位为温度整数部分,低8位保留为0
uint16 humidity; 	//humidity存储湿度,高8位为湿度整数部分,低8位保留为0
extern uint8	hum_h = 0;
uint8	hum_l;
uint8	tem_h;
uint8	tem_l;

//函数声明
void 	delay_us(unsigned char del);
void 	delay_ms(long int s);
unsigned char get_DHT11_value(void);

 

/********************************************************************
 * 函数功能：微秒延时子程序
 * 注意事项：
 * 提示说明：
 * 输    入：整数n，表示延时时间的长短
 * 返    回：
 * *****************************************************************/
void delay_us(unsigned char us)
{
	while(us--);
}

/********************************************************************
 * 函数功能：毫秒延时子程序
 * 注意事项：
 * 提示说明：
 * 输    入：整数n，表示延时时间的长短
 * 返    回：
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

	//无应答信号,退出
	if(DATA_BUS!=0) return 0;         
	//等待电平变高,设置200us超时控制
	time_count=0;
	while(DATA_BUS==0) {        
		//delay_10_us();
		_delay_us(10);
		if(++time_count==20)return 0;
	}         
	//等待电平变低,设置150us超时控制
	time_count=0;
	while(DATA_BUS==1) {        
		//delay_10_us();
		_delay_us(10);
		if(++time_count==15)return 0;
	}

	//数据开始
	while(p!=4) {        
		//低电平宽度50us
		//等待电平变高,设置100us超时控制
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

		//若高电平超过50us,说明数据位为1,否则为0
		if(time_count>=5) {        
			if(p==0) hum_h |=bit;
			if(p==1) hum_l |=bit;
			if(p==2) tem_h |=bit;
			if(p==3) tem_l |=bit;
		}

		bit>>=1;
		//1字节结束
		if(bit==0x00) {        
			p++;
			bit=0x80;
		}
	}

	return 1;
}
 

