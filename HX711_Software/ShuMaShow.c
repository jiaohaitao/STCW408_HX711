//-------------------------------------
//-数码(管共阳极)，驱动程序模块 
//--------------------------------------
//注释添加时间：2014年11月13日
#include "shumashow.h"

//不带小数点的数码管取值数组(0-9)
unsigned char code Tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//带小数点的数码管取值数组(0.-9.)
unsigned char code Tab_Dot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

//内部延时函数，放置与外面冲突，定义成static
static void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<100;j++);
}
//--------------------------------
//-数码管显示函数，
//参数一:unsigned int Second,要显示的秒变量
//参数二:unsigned int show_delay,数码管内部显示延时，不宜过大，也不宜过小
//--------------------------------
void ShuMaShow(unsigned int num,unsigned int show_delay)
{
	
	DIG1=0;DIG2=0;DIG3=1; //个位
	P1=Tab[num%10];
	delay(show_delay);
	
	DIG1=0;DIG2=1;DIG3=0;//十位
	P1=Tab[num/10%10];
	delay(show_delay);
	
	DIG1=1;DIG2=0;DIG3=0;//百位
	P1=Tab[num/100%10];
	delay(show_delay);	
}