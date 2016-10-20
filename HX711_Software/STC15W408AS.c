//-------------------------------------------
//-STC15W408AS��Ŀ������
//-------------------------------------------
//��Ŀע�����ʱ�䣺2014��11��13��

#include "reg51.h"
#include "shumashow.h"	//�������ʾģ�����
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

//�����ӿڶ���
sbit key1= P3^2;
sbit key2= P3^3;

//�̵����ӿڶ���
sbit relay= P2^0;

//��ʱ��ģʽ���üĴ�������
sfr AUXR = 0x8e;                    //Auxiliary register



unsigned char Sys_5ms_flag=0;
unsigned char Sys_10ms_flag=0;
unsigned char Sys_20ms_flag=0;
unsigned char Sys_50ms_flag=0;
unsigned char Sys_100ms_flag=0;
unsigned char Sys_500ms_flag=0;
unsigned char Sys_1000ms_flag=0;

unsigned int Sys_Hx711_Height=0;
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

void Sys_Disp_Height(unsigned int num);

//�������ִ���
struct Button button1;
void BTN1_LONG_RRESS_START_Handler(void* btn);
unsigned char read_button1_pin()
{
	if(key1==0)
	return 0;
	else
	return 1;
}
sfr CLK_DIV     = 0x97;         //ʱ�ӷ�Ƶ�Ĵ���

#define  DEBUG_PRINTF
void DEBUG_Printf(char *s)
{
#ifdef DEBUG_PRINTF
	SendString(s);
#endif
}
//������
void main()
{
    CLK_DIV = 0x00;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��
//  CLK_DIV = 0x01;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/2
//  CLK_DIV = 0x02;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/4
//  CLK_DIV = 0x03;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/8
//  CLK_DIV = 0x04;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/16
//  CLK_DIV = 0x05;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/32
//  CLK_DIV = 0x06;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/64
//  CLK_DIV = 0x07;             //ϵͳʱ��Ϊ�ڲ�R/C��ʱ��/128


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

	//ϵͳ��ʼ��
	delay(10);	//������ʱ


	//P2�˿�����ģʽ���ö���
	//P2M1.n P2M0.n
	//00->standard 01->push-pull
	//10->pure input 11->open drain	
	P2M0=0x01;	 	//P20 �������,�̵�����������
	P2M1=0x00;


	//�������� ��Ч��ƽע��
	button_init(&button1, read_button1_pin, 0);
	//���������¼�ע�ᡢ�ص���������
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
	static unsigned int Sys_5ms_cnt=0;	//��ʱ����̬��������¼��ʱ���жϴ�����5ms����һ��
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