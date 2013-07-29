
#include <stdio.h>
#include "config.h"

extern Fun_idx=FUN_R;   //��ǰ��������
extern Rang_idx=R40;    //��ǰ��������
extern Frq_idx=F1K;     //��ǰƵ������
extern Mode_idx=MODE_S; //��ǰģʽ����
extern Gain_idx=GAIN1;  //��ǰ��������
extern Vxy_idx=VX_A;    //��ǰ��������

extern Actual_Frq;
extern Vxy[];

//�����
code unsigned char gainB[4] =  {  1,   3,   9,  27 };
//���������
code float resistorB[4] = { 40, 1e3, 1e4, 1e5 };


/***********************************************************************
Function : Fmt
Note     : ��ʾLCR
***********************************************************************/
void Fmt(float a)
{
	unsigned char showString[20];
	sprintf(showString, "%.2f", a);
}

/***********************************************************************
Function : showR
Note     : ��ʾLCR
***********************************************************************/
void showR(void){ 
	//��ʾƵ��
	//LCD_Locate(2,0); 
	//if (Frq_idx==F100) LCD_PrintChar('A');
	//if (Frq_idx==F1K) LCD_PrintChar('B');
	//if (Frq_idx==F7K8) LCD_PrintChar('C');
	//��ʾ����
	//LCD_PrintChar(Rang_idx+49);

	//LCD_Locate(1,0);
	if (Mode_idx==MODE_S)//����
	{
		//LCD_PrintString("Zs");

		//LCD_PrintString('R'); 

		//LCD_Locate(2,4);
	}
	if (Mode_idx==MODE_P)//����
	{
		//LCD_PrintString("Zp");
	}
	  
}



/***********************************************************************
Function : calcLCR
Note     : ����LCR
***********************************************************************/
void calcLCR(void){
	xdata float a=0,b=0,c=0;
	xdata float w=0;

	a =  +(Vxy[VX_C] * Vxy[VX_C] + Vxy[VY_D] * Vxy[VY_D] );
	b =  -( Vxy[VX_A] * Vxy[VX_C] + Vxy[VY_B] * Vxy[VY_D]);
	c =  -(Vxy[VY_B] * Vxy[VX_C] - Vxy[VX_A] * Vxy[VY_D]);
	
	a /= resistorB[Rang_idx];  //�����±۵�����ֵ
 
	w = 2*3.1415926* Actual_Frq;
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
Function : absMax
Note     : ȡ����������ֵ�����
***********************************************************************/
int absMax(int a,int b)
{
  if(a<0) a = -a;
  if(b<0) b = -b;
  if(b>a) a = b;
  return a;
}
