//-------------------------------------------
//-STC15W408AS项目主程序
//-------------------------------------------
//项目注释添加时间：2014年11月13日

#include "reg51.h"
#include "shumashow.h"	//数码管显示模块调用
#include "multi_button.h"
#include "uart.h"
#include "led.h"
#include "hx711.h"

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

//按键接口定义
sbit key1= P3^2;
sbit key2= P3^3;

//继电器接口定义
sbit relay= P2^0;

//定时器模式设置寄存器定义
sfr AUXR = 0x8e;                    //Auxiliary register



unsigned char Sys_5ms_flag=0;
unsigned char Sys_10ms_flag=0;
unsigned char Sys_20ms_flag=0;
unsigned char Sys_50ms_flag=0;
unsigned char Sys_100ms_flag=0;
unsigned char Sys_500ms_flag=0;
unsigned char Sys_1000ms_flag=0;

unsigned int Sys_Hx711_Height=0;
//定时器0 延时5ms
void TimerInit(void)
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xA0;		//设置定时初值
	TH0 = 0x15;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0不开始计时
}

//定时器开启
void TimerStart(void)
{
 	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xA0;		//设置定时初值
	TH0 = 0x15;		//设置定时初值
	TR0 = 1;                       	//定时器0开始计时
    ET0 = 1;                       	//使能定时器0中断
    EA = 1;							
}
//定时器停止
void TimerStop(void)
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xA0;		//设置定时初值
	TH0 = 0x15;		//设置定时初值					
	TR0 = 0;                        //定时器0开始计时
    ET0 = 0;                        //使能定时器0中断
    EA = 0;
}
//延时
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<100;j++);
}
//继电器开启
void relay_on()
{
	relay=1;
}
//继电器关闭
void relay_off()
{
	relay=0;
}

void Sys_Disp_Height(unsigned int num);

//按键部分处理
struct Button button1;
void BTN1_LONG_RRESS_START_Handler(void* btn);
unsigned char read_button1_pin()
{
	if(key1==0)
	return 0;
	else
	return 1;
}
sfr CLK_DIV     = 0x97;         //时钟分频寄存器

#define  DEBUG_PRINTF
void DEBUG_Printf(char *s)
{
#ifdef DEBUG_PRINTF
	SendString(s);
#endif
}
//主程序
void main()
{
    CLK_DIV = 0x00;             //系统时钟为内部R/C振荡时钟
//  CLK_DIV = 0x01;             //系统时钟为内部R/C振荡时钟/2
//  CLK_DIV = 0x02;             //系统时钟为内部R/C振荡时钟/4
//  CLK_DIV = 0x03;             //系统时钟为内部R/C振荡时钟/8
//  CLK_DIV = 0x04;             //系统时钟为内部R/C振荡时钟/16
//  CLK_DIV = 0x05;             //系统时钟为内部R/C振荡时钟/32
//  CLK_DIV = 0x06;             //系统时钟为内部R/C振荡时钟/64
//  CLK_DIV = 0x07;             //系统时钟为内部R/C振荡时钟/128


    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

	//系统初始化
	delay(10);	//开机延时


	//P2端口引脚模式配置定义
	//P2M1.n P2M0.n
	//00->standard 01->push-pull
	//10->pure input 11->open drain	
	P2M0=0x01;	 	//P20 推挽输出,继电器控制引脚
	P2M1=0x00;


	//按键引脚 有效电平注册
	button_init(&button1, read_button1_pin, 0);
	//按键触发事件注册、回调函数引入
	button_attach(&button1, LONG_RRESS_START, BTN1_LONG_RRESS_START_Handler);

	UartInit();

	TimerInit();   //5ms interrupt

	DEBUG_Printf("\r\n HX711 Comming....\r\n");

	
	while(1)
	{
		if(Sys_5ms_flag>=1){
		   Sys_5ms_flag=0;
		   button_ticks();
		}
	
		if(Sys_10ms_flag>=2){
		   Sys_10ms_flag=0;
	
		}
	
		if(Sys_20ms_flag>=4){
		   Sys_20ms_flag=0;
		   
		}
	
		if(Sys_50ms_flag>=10){
		  Sys_50ms_flag=0;
		  Sys_Disp_Height(Sys_Hx711_Height);
		 
		}
		
		if(Sys_100ms_flag>=20){
		   Sys_100ms_flag=0;
		  
		}
		
		if(Sys_500ms_flag>=100){
		   Sys_500ms_flag=0;
		  
		}
		
		if(Sys_1000ms_flag>=200){
		   Sys_1000ms_flag=0;
		   led1_on_off();
		   led2_on_off();
		  
		}		
	}
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
	static unsigned int Sys_5ms_cnt=0;	//定时器静态变量，记录定时器中断次数，5ms计数一次
	static unsigned int Sys_10ms_cnt=0;
	static unsigned int Sys_20ms_cnt=0;
	static unsigned int Sys_50ms_cnt=0;
	static unsigned int Sys_100ms_cnt=0;
	static unsigned int Sys_500ms_cnt=0;
	static unsigned int Sys_1000ms_cnt=0;

	if(Sys_5ms_cnt>=1){
	   Sys_5ms_cnt=0;
	   Sys_5ms_flag=1;
	}

	if(Sys_10ms_cnt>=2){
	   Sys_10ms_cnt=0;
	   Sys_10ms_flag=1;
	}

	if(Sys_20ms_cnt>=4){
	   Sys_20ms_cnt=0;
	   Sys_20ms_flag=1;
	}

	if(Sys_50ms_cnt>=10){
	  Sys_50ms_cnt=0;
	  Sys_50ms_flag=1;

	}
	
	if(Sys_100ms_cnt>=20){
	   Sys_100ms_cnt=0;
	   Sys_100ms_flag=1;
	}
	
	if(Sys_500ms_cnt>=100){
	   Sys_500ms_cnt=0;
	   Sys_500ms_flag=1;
	}
	
	if(Sys_1000ms_cnt>=200){
	   Sys_1000ms_cnt=0;
	   Sys_1000ms_flag=1;
	}		
	
	Sys_5ms_cnt++;	
	Sys_10ms_cnt++;
	Sys_20ms_cnt++;
	Sys_50ms_cnt++;
	Sys_100ms_cnt++;
	Sys_500ms_cnt++;
	Sys_1000ms_cnt++;		
}

void Sys_Disp_Height(unsigned int num)
{
	ShuMaShow(num,10);
}
void BTN1_LONG_RRESS_START_Handler(void* btn)
{
    //do something...
	DEBUG_Printf("Button1 long press strart\r\n");
}