
#include "config.h"
#include "key.h"
#include "adc.h"
#include "pwm.h"
#include "eeprom.h"
#include "mylcr.h"



extern Trg;
extern Cont;
extern cnt_plus;

unsigned char  Status=STOP;          //ϵͳ״̬

xdata unsigned char Fun_idx=FUN_R;   //��ǰ��������
xdata unsigned char Rang_idx=R40;    //��ǰ��������
xdata unsigned char Frq_idx=F1K;     //��ǰƵ������
xdata unsigned char Mode_idx=MODE_S; //��ǰģʽ����

xdata unsigned char Gain_idx=GAIN1;  //��ǰ��������
xdata unsigned char Vxy_idx=VX_A;    //��ǰ��������


//�������
xdata int Vxy[6]={0,0,0,0,1,1}; //Vxy[Vx_a,Vy_b,Vx_c,Vy_d,g_up,g_down]
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
void WDTC() { WDT_CONTR =0x3C; }

/***********************************************************************
Function : INTT0 interrupt
Note     : ��ʱ���жϴ�����
***********************************************************************/
void INTT0(void) interrupt 1 using 1
{
	TH0 = 0xF5, TL0 = 0x95;    //1ms��ʱ��װ
        //UpdateTimers();
}

/***********************************************************************
Function : timerInter1
Note     : T1�жϣ�LCR���ݲɼ�
***********************************************************************/
void timerInter1(void) interrupt 3 
{
	int c=0;
	TH1 = 0x96, TL1 = 0xAB;      //20ms��ʱ��װ
 
	if (Status == METER)
	{
	  Vxy[Vxy_idx] = ADC_Read();   //��ȡ��ѹֵ

	  if ((Vxy_idx==VX_A)||(Vxy_idx==VX_C))
	  {//��ѹģֵ���ܷ�Ӧ�˷ŵ�������ȣ����������л��жϵ���ģֵ
	    if (Vxy_idx==VX_A) c = absMax(Vxy[VX_C],Vxy[VY_D]); //�л����±�
	    if (Vxy_idx==VX_C) c = absMax(Vxy[VX_A],Vxy[VY_B]); //�л����ϱ�

  	    if (c>VFULL) { setGain(Gain_idx--);}  //��С����
	    else if (c<GAD*1) {setGain(GAIN27);}  //����27��
	    else if (c<GAD*3) {setGain(GAIN9);}   //����9��
	    else if (c<GAD*9) {setGain(GAIN3);}   //����3��

	    if (Vxy_idx==VX_A) Vxy[GAIN_DOWN] = Gain_idx;  //��¼�±�����
            if (Vxy_idx==VX_C) Vxy[GAIN_UP] = Gain_idx;    //��¼�ϱ�����
	   }
 
	  if (Vxy_idx == VY_D) Status = DISPLAY;

 	  Vxy_idx = (Vxy_idx + 1)%4;

	  if (Vxy_idx==VX_A) K3=1;  //�л����ϱ�
	  if (Vxy_idx==VX_C) K3=0;  //�л����±�
	}

	KeyRead();
    
    if (Trg&KEY_F) {setFrq(Frq_idx);}  //����Ƶ��
}

/***********************************************************************
Function : Time_Init
Note     : ��ʱ����ʼ�� 
***********************************************************************/
void Time_Init(void)
{     
     TCON=0, TMOD=0x11; //��T0��T1��Ϊ16��װλ��ʱ��
     TH1 = 0x96, TL1 = 0xAB;    //20ms��ʱ
     TR1=1;  //T1��ʼ����
     TH0 = 0xF5, TL0 = 0x95;    //1ms��ʱ
     TR0=0;  //T0��ͣ����
	
     ET1=1;  //T1���ж�
     ET0=1;  //T0���ж�
}

/***********************************************************************
Function : Main
Note     : ������
***********************************************************************/
void main()
{
    Time_Init();    //��ʱ����ʼ��

    EA=1;           //�����ж�

    ADC_Init(0);    //����ADת��ͨ��ΪP1.0

    P1M0 = 0xFC;    //P1.234567��Ϊ�ƻ���
    P1M1 = 0x03;    //P1.01��Ϊ���迹
    P2M0 = 0xFF;    //P2.01234567��Ϊ�������
    P2 = 0x0F; 

    PWM_Init();     //DDS��ʼ�� ��PCA��ΪPWM
	
    set90(2);       //��ʼ������λ
    setRng(Rang_idx);  //��ʼ��������
    setGain(Gain_idx); //��ʼ��������
    setFrq(Frq_idx);   //DDS��ʼ����Ϊ1kHz	

    //LCD_INIT();

    //LCD_PrintString("LCR 3.0");
    //LCD_2ndRow();
    //LCD_PrintString("XJW Putian, 2012");

    while(1)
     {
  	   WDTC(); 

	   //���̴��� 
   	   if (Trg&KEY_M) {M_Done(Fun_idx);}  //�����л�
   	   if (Trg&KEY_X) {X_Done(Mode_idx);} //ģʽ�л�
   	   if (Trg&KEY_R) {setRng(0);}        //���̲���
   	   if (Trg&KEY_C) {C_Done(0);}        //����
   	   if (Cont&KEY_M)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100)  {cnt_plus=0;M_Done(FUN_S);}
     	    } 
   	   if (Cont&KEY_R)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100) {cnt_plus=0;setRng(1);} 
     	    }
   	   if (Cont&KEY_C)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100) {cnt_plus=0;C_Done(1);}
     	    }
   	   if (!(Cont)){cnt_plus=0;}

	   //��ʾ����
	   if (Status == DISPLAY ) 
	    {
	        Status = METER;

		   //calcLCR();
		   //SendSem(meter_fine);
		   //showR();
              } 
     }
}

