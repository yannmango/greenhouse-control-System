/********************************************************************
 * AVR单片机常用符号定义头文件
 * 功    能：将程序中常用的关键字等用宏定义表示出来，便于理解和编程
 * 作    者：吴威
 * 目标器件：ATmega162v						                          
 * 晶    振: RC 8MHZ								                  
 * 编译环境：ICCAVR 7						                    
 * 版    本：V1.0 (2014/06/26)
 * 邮    箱：wuweicsu@163.com 
 * 时    间：2012年06月26日
 * 来    源：中南大学2011级物联网工程专业1103班
 *【版权】   Copyright(C) CSU IOT V587  All Rights Reserved
 *【声明】   此程序仅用于学习与参考，引用请注明版权和作者信息！
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

#define  nop()	 NOP()									//一个空操作，延时作用，一般为0.4微秒

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


//位操作
#define 	SETBIT(val, bitn)     (val |= (1<<(bitn)))  //设置
#define 	CLRBIT(val, bitn)     (val &=~(1<<(bitn)))  //清除
#define 	GETBIT(val, bitn)     ((val>>bitn)&0x01)	//获取
#define 	REVERBIT(val, bitn)   (val ^= (1<<(bitn))	//取反

#endif

