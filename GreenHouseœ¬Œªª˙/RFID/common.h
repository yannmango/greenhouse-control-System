/********************************************************************
 * AVR��Ƭ�����÷��Ŷ���ͷ�ļ�
 * ��    �ܣ��������г��õĹؼ��ֵ��ú궨���ʾ�������������ͱ��
 * ��    �ߣ�����
 * Ŀ��������ATmega162v						                          
 * ��    ��: RC 8MHZ								                  
 * ���뻷����ICCAVR 7						                    
 * ��    ����V1.0 (2014/06/26)
 * ��    �䣺wuweicsu@163.com 
 * ʱ    �䣺2012��06��26��
 * ��    Դ�����ϴ�ѧ2011������������רҵ1103��
 *����Ȩ��   Copyright(C) CSU IOT V587  All Rights Reserved
 *��������   �˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��
 *******************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

typedef unsigned	char	uint8;
typedef unsigned	short	uint16;
typedef unsigned	int		uint32;
typedef	signed		char	sint8;
typedef	signed		short	sint16;
typedef signed		long		sint32;

#define F_CPU 8000000UL

#define		BIT0	0x01
#define		BIT1	0x02
#define		BIT2	0x04
#define		BIT3	0x08
#define		BIT4	0x10
#define		BIT5	0x20
#define		BIT6	0x40
#define		BIT7	0x80

#define  nop()	 NOP()									//һ���ղ�������ʱ���ã�һ��Ϊ0.4΢��

#define	FALSE	0
#define	TRUE	1

/**
 *	#define  NO_CARD			0
 *	#define  NO_REG_CARD	    1
 *	#define  REG_CARD			2
 *	#define  NEW_CARD			3
 *
 *	#define  SET_PASSWD    0
 *	#define  DEF_PASSWD    1
 *	#define  CARD_PASSWD   2
*/


//λ����
#define 	SETBIT(val, bitn)     (val |= (1<<(bitn)))  //����
#define 	CLRBIT(val, bitn)     (val &=~(1<<(bitn)))  //���
#define 	GETBIT(val, bitn)     ((val>>bitn)&0x01)	//��ȡ
#define 	REVERBIT(val, bitn)   (val ^= (1<<(bitn))	//ȡ��

#endif

