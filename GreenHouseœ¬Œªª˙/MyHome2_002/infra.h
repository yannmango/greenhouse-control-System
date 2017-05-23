#ifndef __SENSOR__
#define __SENSOR__

#include <avr/iom162.h>


//热源红外传感器相关函数
#define	INF_BUS		4
//#define	INF		0x05     //红外


unsigned char get_inf(void);

#endif
