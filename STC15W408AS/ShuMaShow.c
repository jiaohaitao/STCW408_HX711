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
//参数一:unsigned int Second,要显示的秒变量，四位数码管低两位是秒位
//高两位是分位，中间以逗号隔开，可显示0s-(100*60-1)之间的任何时间秒
//参数二:unsigned int show_delay,数码管内部显示延时，不宜过大，也不宜过小
//--------------------------------
void ShuMaShow(unsigned int Second,unsigned int show_delay)
{
	DIG1=0;DIG2=0;DIG3=0;DIG4=1;//选通数码管的4位 最右面，显示秒
	P1=Tab[Second%60%10];
	delay(show_delay);
	
	DIG1=0;DIG2=0;DIG3=1;DIG4=0; //右面第二位，十秒
	P1=Tab[Second%60/10];
	delay(show_delay);
	
	DIG1=0;DIG2=1;DIG3=0;DIG4=0;//右面第三位，分. 带小数点
	P1=Tab_Dot[Second/60%10];
	delay(show_delay);
	
	DIG1=1;DIG2=0;DIG3=0;DIG4=0;//右面第四位，十分
	P1=Tab[Second/60/10];
	delay(show_delay);	
}