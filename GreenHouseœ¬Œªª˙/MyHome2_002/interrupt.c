/********************************************************************
 * 串口功能测试程序
 * 功能：	 利用中断方式实现串口通讯
 * 目标器件：ATmega162v						                          
 * 晶振:     RC 8MHZ								                  
 * 编译环境：AVR Studio 4				                    
 * 版本：    V1.0 (2012/08/09)
 * 网站：    www.briup.com 
 * 邮箱：    iot@briup.com 
 * 时间：    2012年08月09日
 * 来源：    杰普软件科技有限公司
 *【版权】   Copyright(C) 杰普软件 www.briup.com  All Rights Reserved
 *【声明】   此程序仅用于学习与参考，引用请注明版权和作者信息！
 *******************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom162.h>
#include <avr/interrupt.h>

#include "infra.h"
#include "common.h"
#include "interrupt.h"


//发送、接收数据缓冲区设置
#define UART_RX_BUFFER_SIZE 16 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )  
#define UART_TX_BUFFER_SIZE 16 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )

 
//缓冲区定义
static volatile uchar UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile uchar UART_RxHead;
static volatile uchar UART_RxTail;
static volatile uchar UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile uchar UART_TxHead;
static volatile uchar UART_TxTail;


/********************************************************************
 * 函数功能：串口0初始化，8位数据位、1位停止位、无校验、波特率9600
 * 注意事项：
 * 提示说明：需要设置串口开中断，包括接收中断和发送中断
 * *****************************************************************/
void usart_init( void )
{
    uchar x;
    UCSR0A=0x00;			//单倍速模式
	UCSR0B=0xD8;			//接收中断和发送中断使能、接收使能和发送使能
	UCSR0C=0x86;
				//异步通信无校验，8位数据位和1位停止位
	//初始化缓冲区
	UBRR0H=baud_h;
	UBRR0L=baud_l;
	
	//初始化缓冲区
    x = 0; 
    UART_RxTail = x;
    UART_RxHead = x;
    UART_TxTail = x;
    UART_TxHead = x;
}


/********************************************************************
 * 函数功能：串口接收中断处理函数
 * 注意事项：
 * *****************************************************************/
ISR(USART0_RXC_vect)
{
    uchar data;
    uchar tmphead;
    data = UDR0; //读取串口接收寄存器中收到的数据
	
    tmphead = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;//计算缓冲区偏移量,不超过UART_RX_BUFFER_MASK
    UART_RxHead = tmphead; //设置新的偏移量
    if ( tmphead == UART_RxTail ) {
		//错误：缓冲区溢出
    }
    UART_RxBuf[tmphead] = data; //将读到数据写入缓冲区 
}


/********************************************************************
 * 函数功能：串口发送中断处理函数
 * 注意事项：
 * *****************************************************************/
ISR(USART0_UDRE_vect)
{
    uchar tmptail;

	// 发送缓冲池不为空
    if ( UART_TxHead != UART_TxTail ) 
	{
		// 计算偏移量
        tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmptail; //回写偏移量
        UDR0 = UART_TxBuf[tmptail]; //将缓冲池的一个字节放入串口数据发送寄存器UDR0
    } 

   //发送缓冲池为空
	else 
	{
        UCSR0B &= ~(1<<UDRIE0); //禁止发送中断
    }
}


/********************************************************************
 * 函数功能：从接收缓冲区即循环等待队列取出一个字节
 * 注意事项：
 * *****************************************************************/
uchar getch( void )
{
    uchar tmptail;
	//接收缓冲区为空 则阻塞
    while ( UART_RxHead == UART_RxTail ) ;

    tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;
    UART_RxTail = tmptail;
    return UART_RxBuf[tmptail];//相当于返回接收缓冲池即等待队列的第一个字节
}


/********************************************************************
 * 函数功能：往发送缓冲池写一个字节
 * 注意事项：
 * *****************************************************************/
void putch( uchar data )
{
    uchar tmphead;
    tmphead = ( UART_TxHead + 1 ) & UART_TX_BUFFER_MASK;
    while ( tmphead == UART_TxTail ) ;
    UART_TxBuf[tmphead] = data; 
    UART_TxHead = tmphead; 

    UCSR0B |= (1<<UDRIE0); //发送中断使能 自动进入发送中断处理函数
}



//发送帧 4个字节
void  putframe(FRAME frame[]){

	uint i;
	for(i=0;i<FRAME_LEN;i++)
	{
		putch(frame[i]);
	}
}


/*
#define FRAME_HEAD         0
#define FRAME_SENSORID     1
#define FRAME_DATA_1       2
#define FRAME_CHECK        3
*/


//求校验字节
uchar check(FRAME frame[]){
	
   uchar check = 0x00;

   check ^= frame[FRAME_HEAD];
   check ^= frame[FRAME_SENSORID];
   check ^= frame[FRAME_DATA_1];

   return check;
}


