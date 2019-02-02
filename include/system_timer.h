#ifndef __SYSTEM_TIMER__H
#define __SYSTEM_TIMER__H

#include "aduc812.h"


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long time;

void SetVector(unsigned char __xdata * Address, void * Vector);
void initialize_system_timer(void);
time get_ms(void);
time get_ms_after(time t0);
void delay_ms(time t);

#endif