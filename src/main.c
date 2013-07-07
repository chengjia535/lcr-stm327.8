
#include <reg52.h>
#include "config.h"
#include "ptask.h"
#include "mylcr.h"
#include "key.h"
#include "adc.h"
#include "pwm.h"
#include "hd44780.h"

SEM InitSem(meter_fine)

xdata unsigned char Fun_idx=FUN_R;   //��ǰ��������
xdata unsigned char Rang_idx=R40;    //��ǰ��������
xdata unsigned char Frq_idx=F1K;     //��ǰƵ������
xdata unsigned char Mode_idx=MODE_S; //��ǰģʽ����
xdata unsigned char Gain_idx=GAIN1;  //��ǰ��������
xdata unsigned char Vxy_idx=VX_A;    //��ǰ��������

//�����
code float gainB[4] =  {  1,   3,   9,  27 };
//���������
code float resistorB[4] = { 40, 1e3, 1e4, 1e5 };
//�������
xdata int Vxy[12]={0,0,0,0,1,1}; //Vxy[Vx_a,Vy_b,Vx_c,Vy_d,g1,g2]
//��������

xdata struct Ida{
 char zo[3];//����Ƶ���µ�������ֵ
 char j1;   //��λ����(3����)
 char j2;   //��λ����(10����)
 char J[4];  //��λ����(V/I�任��)
 char R[4]; //�±۵�������(40,1k,10k,100k)
 char g1;   //��������(3����)
 char g2;   //��������(10����)
 char phx; //1kHz������λ����
 char R4b; //100k��7.8kHzƵ���µķ��Ȳ���
 char G2b; //9����7.8kHzƵ���µķ��Ȳ���
 char feq; //Ƶ������
 char ak;  //ADб������
 float QRs[3],QXs[3]; //��·��������
 float QRo[3],QXo[3]; //��·��������
} cs;

/***********************************************************************
Function : WDTC
Note     : ������Ź�
***********************************************************************/
void WDTC()
{ WDT_CONTR =0x3C; }

/***********************************************************************
Function : INTT0 interrupt
Note     : ��ʱ���жϴ�����
Updata   : 2013-06-18
***********************************************************************/
void INTT0(void) interrupt 1 using 1
{
	TH0 = 0xF5, TL0 = 0x95;    //1ms��ʱ��װ
        UpdateTimers();
}


/***********************************************************************
Function : setGain
Note     : ���õ�·����
***********************************************************************/
void setGain(unsigned char switch_sts){
  if (switch_sts!=Gain_idx) { Gain_idx=switch_sts; }
  else  { Gain_idx = (Gain_idx+1)%4; }
 
  K4 = Gain_idx & 2, K6 = Gain_idx & 1; //1��--27��
}


/***********************************************************************
Function : Init
Note     : ��ʼ�� 
***********************************************************************/
void Init(void)
{	//��ʱ����ʼ��
        TCON=0, TMOD=0x11; //��T0��Ϊ16λ��ʱ����T1��Ϊ16λ��ʱ��
        TH1 = 0, TL1 = 0;
        TR1=1;  //T1��ʼ����
	TH0 = 0xF5, TL0 = 0x95;    //1ms��ʱ
        TR0=1;  //T0��ʼ����
 	ET1=1;  //T1���ж�
	ET0=1;  //T0���ж�

	ADC_Init(0);    //����ADת��ͨ��ΪP1.0

 	P1M0 = 0xFC;    //P1.234567��Ϊ�ƻ���
 	P1M1 = 0x03;    //P1.01��Ϊ���迹
 	P2M0 = 0xFF;    //P2.01234567��Ϊ�������
 	P2 = 0x0F; 

	PWM_Init();   //DDS��ʼ�� ��PCA��ΪPWM
	
	EA=1;   //�����ж�

 	set90(2);   //��ʼ������λ
 	setRng(Rang_idx);  //��ʼ��������
 	setGain(Gain_idx); //��ʼ��������
 	setFrq(Frq_idx);    //DDS��ʼ����Ϊ1kHz	

	LCD_INIT();

	LCD_PrintString("LCR 3.0");
        LCD_2ndRow();
	LCD_PrintString("XJW Putian, 2012");
}

/***********************************************************************
Function : calcLCR
Note     : ����LCR
***********************************************************************/
void calcLCR(void){
	xdata float a=0,b=0,c=0;

	a =  +(Vxy[VX_C] * Vxy[VX_C] + Vxy[VY_D] * Vxy[VY_D] );
	b =  -( Vxy[VX_A] * Vxy[VX_C] + Vxy[VY_B] * Vxy[VY_D]);
	c =  -(Vxy[VY_B] * Vxy[VX_C] - Vxy[VX_A] * Vxy[VY_D]);
	
	a /= resistorB[Rang_idx];  //�����±۵�����ֵ
 
}

/***********************************************************************
Function : timerInter1
Note     : T1�жϣ�LCR���ݲɼ�
***********************************************************************/
void timerInter1(void) interrupt 3 {
	Vxy[Vxy_idx] = ADC_Read();

//	WaitSem(meter_fine);
 	Vxy_idx = (Vxy_idx + 1)%4;
	set90(Vxy_idx);
	if (Vxy_idx==VX_A) K3=1;
	if (Vxy_idx==VX_C) K3=0;

}

/***********************************************************************
Function : showR
Note     : ��ʾLCR
***********************************************************************/
void showR(void){ 
	//��ʾƵ��
	LCD_Locate(2,0);
	if (Frq_idx==F100) LCD_PrintChar('A');
	if (Frq_idx==F1K) LCD_PrintChar('B');
	if (Frq_idx==F7K8) LCD_PrintChar('C');
	//��ʾ����
	if (Mode_idx==MODE_S)//����
	{
		LCD_Locate(1,0);
		LCD_PrintString("Zp");
	}
	if (Mode_idx==MODE_P)//����
	{
		LCD_Locate(1,0);
		LCD_PrintString("Zs");
	}
	  
}

/***********************************************************************
Function : Main
Note     : ������
***********************************************************************/
void main()
{
    Init();
    	
	while(1)
	{
  	   WDTC(); 
	   RunTaskA(KeyProc,Task1_ID);
	   if (meter_fine) 
	   {
		   calcLCR();
		   SendSem(meter_fine);
	   }
	   showR();
	}
}

