
#include <avr/io.h>
#include <util/delay.h>
#include  "common.h"
#include  "mytem.h"

  
int T3;

//微秒级延时
void delayus(uchar del)
{
	while(del--);
}
 

/********************************************************************
 * 函数功能：启动温度传感器LM75
 * 注意事项：
 * 提示说明：用于产生TWI(IIC)总线的起始信号
 * *****************************************************************/
void	init_lm75(void)
{
	//设置引脚方向
	SETBIT(DDRC,LM75_SDA);	  //设置IO 位输出
	SETBIT(DDRC,LM75_SCL);
	//空闲状态 SDA、SCL均为高电平
	SETBIT(PORTC,LM75_SDA);	  //输出  置1
	_delay_us(10);
	SETBIT(PORTC,LM75_SCL);
	_delay_us(10);
	//发出起始信号，时钟高电平时产生下降沿
	CLRBIT(PORTC,LM75_SDA); //输出  清0
	_delay_us(20);
	CLRBIT(PORTC,LM75_SCL);
	_delay_us(20);
}


/********************************************************************
 * 函数功能：单片机发送一字节数据给LM75
 * *****************************************************************/
uchar sendbyte_lm75(uchar buf)
{
	uchar i = 0;
	SETBIT(DDRC,LM75_SDA);//设置IO为输出
	SETBIT(DDRC,LM75_SCL);				
	//将数据串行输出
	for(i = 8;i > 0;i--){
		CLRBIT(PORTC,LM75_SCL);
		_delay_us(20);
		if(buf & 0x80)		 //bit7 == 1
			SETBIT(PORTC,LM75_SDA);  		  
		else					 //bit7 == 0
			CLRBIT(PORTC,LM75_SDA);
		buf <<= 1;
		SETBIT(PORTC,LM75_SCL);
		_delay_us(20);
	}
	//等待lm75发出应答信号
	CLRBIT(PORTC,LM75_SCL);
	_delay_us(20);//wait for ack
	//释放数据线
	SETBIT(PORTC,LM75_SDA);
	_delay_us(5);
	//修改SDA引脚为输入，准备接受应答信号
	CLRBIT(DDRC,LM75_SDA);	//set SDA  input	
	SETBIT(PORTC,LM75_SCL);		
	_delay_us(20);

	if(GETBIT(PINC,LM75_SDA)){//fail
		SETBIT(DDRC,LM75_SCL);
		CLRBIT(PORTC,LM75_SCL);
		return 0xEE;
	}else{//成功返回 地址
		SETBIT(DDRC,LM75_SCL);
		CLRBIT(PORTC,LM75_SCL);
		return LM75_RD_ADDRESS;
	}

}


/****************************************************************
 * 函数功能：单片机从LM75获取一字节数据函数
 * **************************************************************/
uchar getbyte_lm75(void)
{
	uchar i = 0,buf = 0;
	CLRBIT(DDRC,LM75_SDA);//数据输入
	SETBIT(DDRC,LM75_SCL);//时钟输出
			 
	for(i = 0;i < 8;i++){

		/*置时钟线为低，准备接收数据位*/
		CLRBIT(PORTC,LM75_SCL);
		delayus(20);

		/*置时钟线为高使数据线上数据有效*/
		SETBIT(PORTC,LM75_SCL);
		delayus(10);
		buf <<= 1;

		/*读数据位,接收的数据位放入buf中*/
		if(GETBIT(PINC,LM75_SDA)){
			buf += 1;
			delayus(10);
		}
		CLRBIT(PORTC,LM75_SCL);		
	}
	return buf;
}



/*****************************************************************
 * 函数功能：单片机发送停止信号
 * **************************************************************/
void stop_lm75(void)
{
	//设置IO方向
	SETBIT(DDRC,LM75_SDA);
	SETBIT(DDRC,LM75_SCL);
	//初始信号线为低
	CLRBIT(PORTC,LM75_SDA);
	delayus(20);
	//发送停止信号
	SETBIT(PORTC,LM75_SCL);
	delayus(20);
	SETBIT(PORTC,LM75_SDA);
	delayus(20);
}


/****************************************************************
 * 函数功能：单片机进行应答信号 
 * **************************************************************/
void ack_lm75(uchar ack)
{
	SETBIT(DDRC,LM75_SDA);
	SETBIT(DDRC,LM75_SCL);

	if(ack)
		CLRBIT(PORTC,LM75_SDA);
	else
		SETBIT(PORTC,LM75_SDA);

	delayus(8);
	SETBIT(PORTC,LM75_SCL);
	delayus(20);
	CLRBIT(PORTC,LM75_SCL);
	delayus(10);
}


/****************************************************************
 * 函数功能：单片机获取温度值
 * **************************************************************/
uchar temperature_lm75(void)
{
 	uchar temp[2];
	uchar i;

	//发送起始信号
	init_lm75();

	//发送LM75的地址:读写位 1001 A2 A1 A
	T3=sendbyte_lm75(0x95) ;

    //整数字节
	temp[1] = getbyte_lm75();
	ack_lm75(1);

    //小数字节
	temp[0] = getbyte_lm75();
	ack_lm75(0);
  
    //停止
	stop_lm75();


	i = 0;
	i = temp[1]&0x80;    //1000 0000  i得到temp[1]的第8位
	temp[1] <<= 1;      //左移一位
	temp[1] |= (temp[0]>>7); //将temp[0]的最高位 作为 temp[1]的最低位

	if(i){//负数转码
		temp[1] = ~temp[1]+1;
	}
   
    //下面这两步都交给上层处理
    //整数字节乘以 0.5 这样会损失精度
   // temp[1]=temp[1]>>1;

    //装换成10进制数
	//temp[1] = Hex_Decimal(temp[1]);

	return temp[1];
 
}
