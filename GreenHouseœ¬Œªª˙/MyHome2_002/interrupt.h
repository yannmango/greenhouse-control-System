#ifndef __INTERRUPT__
#define __INTERRUPT__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/iom162.h>
#include <avr/interrupt.h>

#include "common.h"

#define F_CPU 8000000
#define baud 9600
#define baud_setting (unsigned int)((unsigned long)F_CPU/(16*(unsigned long)baud)-1)
#define baud_h	((uchar)(baud_setting>>8))
#define baud_l  ((uchar)(baud_setting))


//º¯ÊıÉùÃ÷
uchar 	getch(void);
void 	putch(uchar data);
void    puts(uchar str[]);
void 	usart_init(void);


#endif
