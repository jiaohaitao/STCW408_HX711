#ifndef _HX711_H
#define _HX711_H
#include "reg51.h"

sbit HX711_DOUT=	P2^6   ;
sbit HX711_CLK=	P2^7	   ;
void HX711_Init(void);
unsigned int HX711_Read(void);
#endif