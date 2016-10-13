//-------------------------------------------
//-STC15W408AS��Ŀ������
//-------------------------------------------
//��Ŀע�����ʱ�䣺2014��11��13��

#include "reg51.h"
#include "shumashow.h"	//�������ʾģ�����


//P2�˿�����ģʽ���ö���
sfr P2M1=0x95;
sfr P2M0=0x96;
//P2M1.n P2M0.n
//00->standard 01->push-pull
//10->pure input 11->open drain

//�����ӿڶ���
sbit key1= P3^2;
sbit key2= P3^3;

//led�ӿڶ���
sbit led1= P2^3;
sbit led2= P2^2;
sbit led3= P2^1;

//�̵����ӿڶ���
sbit relay= P2^0;

//��ʱ��ģʽ���üĴ�������
sfr AUXR = 0x8e;                    //Auxiliary register



//��ʱ��0 ��ʱ5ms
void TimerInit(void)
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xA0;		//���ö�ʱ��ֵ
	TH0 = 0x15;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0����ʼ��ʱ
}

//��ʱ������
void TimerStart(void)
{
 	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xA0;		//���ö�ʱ��ֵ
	TH0 = 0x15;		//���ö�ʱ��ֵ
	TR0 = 1;                       	//��ʱ��0��ʼ��ʱ
    ET0 = 1;                       	//ʹ�ܶ�ʱ��0�ж�
    EA = 1;							
}
//��ʱ��ֹͣ
void TimerStop(void)
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xA0;		//���ö�ʱ��ֵ
	TH0 = 0x15;		//���ö�ʱ��ֵ					
	TR0 = 0;                        //��ʱ��0��ʼ��ʱ
    ET0 = 0;                        //ʹ�ܶ�ʱ��0�ж�
    EA = 0;
}
//��ʱ
void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<100;j++);
}
//�̵�������
void relay_on()
{
	relay=1;
}
//�̵����ر�
void relay_off()
{
	relay=0;
}
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
//led3����
void led3_on()
{
	led3=0;
}
//led3�ر�
void led3_off()
{
	led3=1;
}

//-------------�������ƿ��غͼ�������-----------------------
unsigned int use_cnt=0;		//�������Ʊ��� 
//#define USE_TEST	//���ÿ��� 
//---------------------------------------------------------
unsigned int RelayOnMin=0;	//�̵�����ʱ����ʱ��
unsigned int RelayOrderMin=0;//�̵���ԤԼ��ʱʱ��

unsigned int OneSecondFlag=0;

unsigned char System_Mode=0x01;	//��ʱ����ģʽ
													//0x02;//ԤԼ����ģʽ
													//0x03;//ԤԼģʽ
													//0x04//��ʱģʽ

void RelayOnTimeSetMode(void);
void RelayOrderTimeSetMode(void);
void RelayOrderMode(void);
void RelayOnMode(void);
//������
void main()
{
	//ϵͳ��ʼ��
	delay(10);	//������ʱ
	
	P2M0=0x01;	 	//P20 �������,�̵�����������
	P2M1=0x00;
	relay=0;	
	led1_off();
	led2_off();
	led3_off();
	TimerInit();   //5ms interrupt
	TimerStart();
	
	System_Mode=0x01;//Ĭ�Ͻ��붨ʱ����ģʽ
	while(1)
	{
		if(System_Mode==0x01)//����ģʽ
		{
		//	NormalMode_Key_Scan();
			RelayOnTimeSetMode();
			ShuMaShow(RelayOnMin,10);
		}
		else if(System_Mode==0x02)//ԤԼ����ģʽ
		{
			RelayOrderTimeSetMode();
		}
		else if(System_Mode==0x03)//ԤԼģʽ
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
	static unsigned int i=0;	//��ʱ����̬��������¼��ʱ���жϴ�����5ms����һ��
		
	i++;
	if(i>199)	//200��Ϊ1�� 200*5ms=1s
	{
		i=0;	//��������
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
			System_Mode=0x02;//����ԤԼģʽ
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
			System_Mode=0x03;//����ԤԼģʽ
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
