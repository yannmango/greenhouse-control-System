#ifndef __TEM__
#define __TEM__

#include <avr/iom162.h>
#include "common.h"


//温度传感器相关的函数 
#define LM75_SDA 0 			  	   	  			   	
#define LM75_SCL 1		
#define LM75_OS  2
#define LM75_WR_ADDRESS   0x94      //1001	010 0 写
#define LM75_RD_ADDRESS   0x95 		//1001	010 1 读	
#define LM75_POINT   	  0x00	


void init_lm75(void);
uchar sendbyte_lm75(uchar buf);
uchar getbyte_lm75(void);
void stop_lm75(void);
void ack_lm75(uchar ack);
uchar temperature_lm75(void);


#endif
