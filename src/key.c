
#include <reg52.h>
#include "ptask.h"
#include "config.h"
#include "pwm.h"


#define KEYPort      ~P3^0x1F
#define KEY_M        0x01
#define KEY_X        0x02
#define KEY_R        0x03
#define KEY_F        0x04
#define KEY_C        0x05

unsigned char Trg = 0;
unsigned char Cont = 0;

unsigned char cnt_plus=0;


extern Fun_idx;
extern Frq_idx; 
extern Mode_idx;
extern Rang_idx;

 
/***********************************************************************
Function : KeyRead
Note     : ������ 
***********************************************************************/
void KeyRead(void)
{
   unsigned char ReadData = (KEYPort);
   Trg = ReadData & (ReadData ^ Cont);
   Cont = ReadData;
}

/***********************************************************************
Function : C_Done
Note     : ���㿪�� 
***********************************************************************/
void C_Done(unsigned char switch_sts)
{
	unsigned char i = switch_sts;
  //if (switch_sts<>Cls_idx) Cls_idx=switch_sts;
  //else  { Cls_idx = (Cls_idx+1)%2; }
}

/***********************************************************************
Function : M_Done
Note     : ���ܿ���
***********************************************************************/
void M_Done(unsigned char switch_sts)
{
   if (switch_sts!=Fun_idx) {Fun_idx=switch_sts;}
   else  { Fun_idx = (Fun_idx+1)%3; }
  
}

/***********************************************************************
Function : setRng
Note     : ���̿���
***********************************************************************/
void setRng(unsigned char switch_sts)
{
   if (switch_sts!=Rang_idx) { Rang_idx=switch_sts; }
   else  { Rang_idx = (Rang_idx+1)%4; }

   Ka = Rang_idx & 2, Kb = Rang_idx & 1; 
}

/***********************************************************************
Function : X_Done
Note     : ģʽ���� 
***********************************************************************/
void X_Done(unsigned char switch_sts)
{
   if (switch_sts!=Mode_idx) { Mode_idx=switch_sts; }
   else  { Mode_idx = (Mode_idx+1)%2; }
}

/***********************************************************************
Function : setFrq 
Note     : Ƶ�ʿ��� 
***********************************************************************/
void setFrq(unsigned char switch_sts)
{
  if (switch_sts!=Frq_idx) { Frq_idx=switch_sts; }
  else  { Frq_idx = (Frq_idx+1)%3; }

  if (Frq_idx==F100) { K5=0,K8=1; setDDS(F100); }     //100H�˲�
  if (Frq_idx==F1K) { K5=0,K8=0; setDDS(F1K); }      //1K�˲�
  if (Frq_idx==F7K8) { K5=1;K8=0; setDDS(F7K8); }     //7K8�˲�
}

/***********************************************************************
Function : KeyProc
Note     : ���̴��� 
***********************************************************************/
unsigned char KeyProc(void)
{
   _SS
   KeyRead();
   WaitX(20);
   KeyRead();
   if (Trg&KEY_M) {M_Done(Fun_idx);}  //�����л�
   if (Trg&KEY_X) {X_Done(Mode_idx);} //ģʽ�л�
   if (Trg&KEY_R) {setRng(0);}        //���̲���
   if (Trg&KEY_F) {setFrq(Frq_idx);}  //����Ƶ��
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
   _EE
}










 
