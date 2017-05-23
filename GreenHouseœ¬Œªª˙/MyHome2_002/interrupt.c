/********************************************************************
 * ���ڹ��ܲ��Գ���
 * ���ܣ�	 �����жϷ�ʽʵ�ִ���ͨѶ
 * Ŀ��������ATmega162v						                          
 * ����:     RC 8MHZ								                  
 * ���뻷����AVR Studio 4				                    
 * �汾��    V1.0 (2012/08/09)
 * ��վ��    www.briup.com 
 * ���䣺    iot@briup.com 
 * ʱ�䣺    2012��08��09��
 * ��Դ��    ��������Ƽ����޹�˾
 *����Ȩ��   Copyright(C) ������� www.briup.com  All Rights Reserved
 *��������   �˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��
 *******************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom162.h>
#include <avr/interrupt.h>

#include "infra.h"
#include "common.h"
#include "interrupt.h"


//���͡��������ݻ���������
#define UART_RX_BUFFER_SIZE 16 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )  
#define UART_TX_BUFFER_SIZE 16 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )

 
//����������
static volatile uchar UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile uchar UART_RxHead;
static volatile uchar UART_RxTail;
static volatile uchar UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile uchar UART_TxHead;
static volatile uchar UART_TxTail;


/********************************************************************
 * �������ܣ�����0��ʼ����8λ����λ��1λֹͣλ����У�顢������9600
 * ע�����
 * ��ʾ˵������Ҫ���ô��ڿ��жϣ����������жϺͷ����ж�
 * *****************************************************************/
void usart_init( void )
{
    uchar x;
    UCSR0A=0x00;			//������ģʽ
	UCSR0B=0xD8;			//�����жϺͷ����ж�ʹ�ܡ�����ʹ�ܺͷ���ʹ��
	UCSR0C=0x86;
				//�첽ͨ����У�飬8λ����λ��1λֹͣλ
	//��ʼ��������
	UBRR0H=baud_h;
	UBRR0L=baud_l;
	
	//��ʼ��������
    x = 0; 
    UART_RxTail = x;
    UART_RxHead = x;
    UART_TxTail = x;
    UART_TxHead = x;
}


/********************************************************************
 * �������ܣ����ڽ����жϴ�����
 * ע�����
 * *****************************************************************/
ISR(USART0_RXC_vect)
{
    uchar data;
    uchar tmphead;
    data = UDR0; //��ȡ���ڽ��ռĴ������յ�������
	
    tmphead = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;//���㻺����ƫ����,������UART_RX_BUFFER_MASK
    UART_RxHead = tmphead; //�����µ�ƫ����
    if ( tmphead == UART_RxTail ) {
		//���󣺻��������
    }
    UART_RxBuf[tmphead] = data; //����������д�뻺���� 
}


/********************************************************************
 * �������ܣ����ڷ����жϴ�����
 * ע�����
 * *****************************************************************/
ISR(USART0_UDRE_vect)
{
    uchar tmptail;

	// ���ͻ���ز�Ϊ��
    if ( UART_TxHead != UART_TxTail ) 
	{
		// ����ƫ����
        tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmptail; //��дƫ����
        UDR0 = UART_TxBuf[tmptail]; //������ص�һ���ֽڷ��봮�����ݷ��ͼĴ���UDR0
    } 

   //���ͻ����Ϊ��
	else 
	{
        UCSR0B &= ~(1<<UDRIE0); //��ֹ�����ж�
    }
}


/********************************************************************
 * �������ܣ��ӽ��ջ�������ѭ���ȴ�����ȡ��һ���ֽ�
 * ע�����
 * *****************************************************************/
uchar getch( void )
{
    uchar tmptail;
	//���ջ�����Ϊ�� ������
    while ( UART_RxHead == UART_RxTail ) ;

    tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;
    UART_RxTail = tmptail;
    return UART_RxBuf[tmptail];//�൱�ڷ��ؽ��ջ���ؼ��ȴ����еĵ�һ���ֽ�
}


/********************************************************************
 * �������ܣ������ͻ����дһ���ֽ�
 * ע�����
 * *****************************************************************/
void putch( uchar data )
{
    uchar tmphead;
    tmphead = ( UART_TxHead + 1 ) & UART_TX_BUFFER_MASK;
    while ( tmphead == UART_TxTail ) ;
    UART_TxBuf[tmphead] = data; 
    UART_TxHead = tmphead; 

    UCSR0B |= (1<<UDRIE0); //�����ж�ʹ�� �Զ����뷢���жϴ�����
}



//����֡ 4���ֽ�
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


//��У���ֽ�
uchar check(FRAME frame[]){
	
   uchar check = 0x00;

   check ^= frame[FRAME_HEAD];
   check ^= frame[FRAME_SENSORID];
   check ^= frame[FRAME_DATA_1];

   return check;
}


