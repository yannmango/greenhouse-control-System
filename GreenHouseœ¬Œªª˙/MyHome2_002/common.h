#ifndef __COMMON__
#define __COMMON__


typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int  uint;

typedef unsigned char uint8;
typedef unsigned int  uint16;


#define SETBIT(val, bitn)     (val |= (1<<(bitn)))  //设置 
#define CLRBIT(val, bitn)     (val &=~(1<<(bitn)))  //清除 
#define GETBIT(val, bitn)     ((val>>bitn)&0x01)	//获取 
#define REVERBIT(val, bitn)   (val ^= (1<<(bitn))	//取反


#define FRAME_LEN          4

#define FRAME_HEAD         0
#define FRAME_SENSORID     1
#define FRAME_DATA_1       2
#define FRAME_CHECK        3

#define FRAME   uchar 

#define TEM                0x31
#define DHT                0x32
#define VIB                0x33
#define INF                0x34
#define LIGHT              0x35
#define BELL               0x36
#define MOTOR_SHUN         0x37
#define MOTOR_NI           0x38
#define LED                0x39


/*
//传感器编号

温度          0x01
光敏          0x02
震动          0x03
热红外源      0x04
蜂鸣器        0x05

步进电机_正转 0x06
步进电机_反转 0x07
温湿度_温度   0x08
温湿度_湿度   0x09

*/
#endif
