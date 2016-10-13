//-------------------------------------------
//-STC15W408AS项目主程序
//-------------------------------------------
//项目注释添加时间：2014年11月13日

#include "reg51.h"
#include "shumashow.h"	//数码管显示模块调用


//P2端口引脚模式配置定义
sfr P2M1=0x95;
sfr P2M0=0x96;
//P2M1.n P2M0.n
//00->standard 01->push-pull
//10->pure input 11->open drain

//按键接口定义
sbit key1= P3^2;
sbit key2= P3^3;

//led接口定义
sbit led1= P2^3;
sbit led2= P2^2;
sbit led3= P2^1;

//继电器接口定义
sbit relay= P2^0;

//定时器模式设置寄存器定义
sfr AUXR = 0x8e;                    //Auxiliary register



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
//led1开启
void led1_on()
{
	led1=0;
}
//led1关闭
void led1_off()
{
	led1=1;
}
//led2开启
void led2_on()
{
	led2=0;
}
//led2关闭
void led2_off()
{
	led2=1;
}
//led3开启
void led3_on()
{
	led3=0;
}
//led3关闭
void led3_off()
{
	led3=1;
}

//-------------试用限制开关和计数变量-----------------------
unsigned int use_cnt=0;		//试用限制变量 
//#define USE_TEST	//试用开关 
//---------------------------------------------------------
unsigned int RelayOnMin=0;	//继电器定时工作时间
unsigned int RelayOrderMin=0;//继电器预约定时时间

unsigned int OneSecondFlag=0;

unsigned char System_Mode=0x01;	//定时设置模式
													//0x02;//预约设置模式
													//0x03;//预约模式
													//0x04//定时模式

void RelayOnTimeSetMode(void);
void RelayOrderTimeSetMode(void);
void RelayOrderMode(void);
void RelayOnMode(void);
//主程序
void main()
{
	//系统初始化
	delay(10);	//开机延时
	
	P2M0=0x01;	 	//P20 推挽输出,继电器控制引脚
	P2M1=0x00;
	relay=0;	
	led1_off();
	led2_off();
	led3_off();
	TimerInit();   //5ms interrupt
	TimerStart();
	
	System_Mode=0x01;//默认进入定时设置模式
	while(1)
	{
		if(System_Mode==0x01)//正常模式
		{
		//	NormalMode_Key_Scan();
			RelayOnTimeSetMode();
			ShuMaShow(RelayOnMin,10);
		}
		else if(System_Mode==0x02)//预约设置模式
		{
			RelayOrderTimeSetMode();
		}
		else if(System_Mode==0x03)//预约模式
		{
			RelayOrderMode();
		}
		else if(System_Mode==0x04)
		{
			RelayOnMode();
		}

		
	}	
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
	static unsigned int i=0;	//定时器静态变量，记录定时器中断次数，5ms计数一次
		
	i++;
	if(i>199)	//200次为1秒 200*5ms=1s
	{
		i=0;	//重新置零
		OneSecondFlag=1;
	}
}
void RelayOnTimeSetMode()
{
	unsigned int i=30;
	
	led1=0;
	led2=led3=1;
	
	if(key1==0&&key2==0)
	{
		delay(10);
		if(key1==0&&key2==0)
		{
			System_Mode=0x02;//进入预约模式
			while(key1==0||key2==0)
			{
				led1=led2=led3=~led3;
					delay(1000);
				ShuMaShow(RelayOnMin,10);
			}
		}
		if(System_Mode==0x02)
			return;
	}
	
	if(key1==0)
	{
		delay(10);
		if(key1==0)
		{
			RelayOnMin+=1;
			if(RelayOnMin>(99*60+59))
				RelayOnMin=0;
		}
	}
	
	if(key2==0)
	{
		delay(10);
		if(key2==0)
		{
			if(RelayOnMin>0)
				RelayOnMin--;
		}
	}
	
	while(i--)
	{
		ShuMaShow(RelayOnMin,10);
	}
	
}
void RelayOrderTimeSetMode()
{
	unsigned int i=30;
	
	led2=0;
	led1=led3=1;
	
	if(key1==0&&key2==0)
	{
		delay(10);
		if(key1==0&&key2==0)
		{
			System_Mode=0x03;//进入预约模式
			while(key1==0||key2==0)
			{
				led1=led2=led3=~led3;
				ShuMaShow(RelayOrderMin,10);
				delay(1000);
			}
			
		}
		if(System_Mode==0x03)
			return;
	}
	
	if(key1==0)
	{
		delay(10);
		if(key1==0)
		{
			RelayOrderMin+=1;
			if(RelayOrderMin>(99*60+59))
				RelayOrderMin=0;
		}
	}
	
	if(key2==0)
	{
		delay(10);
		if(key2==0)
		{
			if(RelayOrderMin>0)
				RelayOrderMin--;
		}
	}
	
	while(i--)
	{
		ShuMaShow(RelayOrderMin,10);
	}	
}
void RelayOrderMode()
{
	static unsigned char TempSecond=0;
	led1=1;
	led2=0;
		
	
	
	if(key1==0||key2==0)
	{
		delay(10);
		while(key1==0||key2==0)
		{
			RelayOrderMin=0;
			RelayOnMin=0;
			System_Mode=0x01;
			relay=0;
		}
	}
		
	if(OneSecondFlag)
	{
		OneSecondFlag=0x00;
		TempSecond++;
		
		if(RelayOrderMin==0)
		{
			System_Mode=0x04;
			//relay=1;
		}
		
		if(TempSecond==59)
		{
			TempSecond=0;
			if(RelayOrderMin>0)
			{
				RelayOrderMin--;
			}
			
			if(RelayOrderMin==0)
			{
				System_Mode=0x04;
				//relay=1;
			}
		}
		led3=~led3;
	}
	ShuMaShow(RelayOrderMin,10);
}

void RelayOnMode()
{
	static unsigned char TempSecond=0;
	led1=0;
	led2=1;
			
		if(key1==0||key2==0)
	{
		delay(10);
		while(key1==0||key2==0)
		{
			RelayOrderMin=0;
			RelayOnMin=0;
			System_Mode=0x01;
			relay=0;
		}
	}
	
	
	if(OneSecondFlag)
	{
		OneSecondFlag=0x00;
		
		if(RelayOnMin==0)
			{
				relay=0;
				System_Mode=0x01;
			}
			else
			{
				relay=1;
			}
	
		TempSecond++;
		if(TempSecond==59)
		{
			TempSecond=0;
			if(RelayOnMin>0)
			{
				RelayOnMin--;
			}
			if(RelayOnMin==0)
			{
				relay=0;
				System_Mode=0x01;
			}
		}
		led3=~led3;
	}
	ShuMaShow(RelayOnMin,10);
}
