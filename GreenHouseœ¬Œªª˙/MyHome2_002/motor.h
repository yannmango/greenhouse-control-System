#ifndef __MOTOR__
#define __MOTOR__

#include <avr/iom162.h>
#include "common.h"


//步进电机
void motor_ni(uchar n);
void motor_shun(uchar n);


#endif
