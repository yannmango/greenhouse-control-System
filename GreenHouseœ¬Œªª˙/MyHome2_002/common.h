#ifndef __COMMON__
#define __COMMON__


typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int  uint;

typedef unsigned char uint8;
typedef unsigned int  uint16;


#define SETBIT(val, bitn)     (val |= (1<<(bitn)))  //���� 
#define CLRBIT(val, bitn)     (val &=~(1<<(bitn)))  //��� 
#define GETBIT(val, bitn)     ((val>>bitn)&0x01)	//��ȡ 
#define REVERBIT(val, bitn)   (val ^= (1<<(bitn))	//ȡ��


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
//���������

�¶�          0x01
����          0x02
��          0x03
�Ⱥ���Դ      0x04
������        0x05

�������_��ת 0x06
�������_��ת 0x07
��ʪ��_�¶�   0x08
��ʪ��_ʪ��   0x09

*/
#endif
