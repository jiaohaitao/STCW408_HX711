#include "led.h"

//led1����
void led1_on()
{
	led1=0;
}
//led1�ر�
void led1_off()
{
	led1=1;
}
void led1_on_off()
{
	led1=~led1;
}
//led2����
void led2_on()
{
	led2=0;
}
//led2�ر�
void led2_off()
{
	led2=1;
}
void led2_on_off()
{
	led2=~led2;
}