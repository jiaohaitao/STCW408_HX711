//-------------------------------------
//-����(�ܹ�����)����������ģ�� 
//--------------------------------------
//ע�����ʱ�䣺2014��11��13��
#include "shumashow.h"

//����С����������ȡֵ����(0-9)
unsigned char code Tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//��С����������ȡֵ����(0.-9.)
unsigned char code Tab_Dot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

//�ڲ���ʱ�����������������ͻ�������static
static void delay(unsigned int x)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
		for(j=0;j<100;j++);
}
//--------------------------------
//-�������ʾ������
//����һ:unsigned int Second,Ҫ��ʾ�����������λ����ܵ���λ����λ
//����λ�Ƿ�λ���м��Զ��Ÿ���������ʾ0s-(100*60-1)֮����κ�ʱ����
//������:unsigned int show_delay,������ڲ���ʾ��ʱ�����˹���Ҳ���˹�С
//--------------------------------
void ShuMaShow(unsigned int Second,unsigned int show_delay)
{
	DIG1=0;DIG2=0;DIG3=0;DIG4=1;//ѡͨ����ܵ�4λ �����棬��ʾ��
	P1=Tab[Second%60%10];
	delay(show_delay);
	
	DIG1=0;DIG2=0;DIG3=1;DIG4=0; //����ڶ�λ��ʮ��
	P1=Tab[Second%60/10];
	delay(show_delay);
	
	DIG1=0;DIG2=1;DIG3=0;DIG4=0;//�������λ����. ��С����
	P1=Tab_Dot[Second/60%10];
	delay(show_delay);
	
	DIG1=1;DIG2=0;DIG3=0;DIG4=0;//�������λ��ʮ��
	P1=Tab[Second/60/10];
	delay(show_delay);	
}