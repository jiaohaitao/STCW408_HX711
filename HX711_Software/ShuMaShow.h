#ifndef _SHUMASHOW_H
#define _SHUMASHOW_H
#include "reg51.h"
sbit DIG1 =P2^4;
sbit DIG2 =P2^5;
sbit DIG3 =P2^6;

#define DataBus P1

void ShuMaShow(unsigned int num,unsigned int show_delay);
#endif