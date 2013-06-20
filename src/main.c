
#include <reg52.h>
#include "ptask.h"
 

sfr IAP_CONTR = 0xC7; 
sfr WDT_CONTR = 0xC1;
sbit KEY = P3^2;
sbit LED1= P2^4;  
unsigned char code numtab[16]={ 0x24,0x6F,0xE0,0x62,
                     0x2B,0x32,0x30,0x67,
                     0x20,0x22,0x21,0x38,
                     0xB4,0x68,0xB0,0xB1
                   };
unsigned char keycount = 0;

InitSem(task_Wait_flag)

/***********************************************************************
Function : WDTC
Input    : 
Output   : 
Return   : 
Stacks   : 1
Note     : ������Ź�
Updata   : 2013-06-18
***********************************************************************/
void WDTC()
{ WDT_CONTR =0x3C; }

/***********************************************************************
Function : Init_IO
Input    : 
Output   : 
Return   : 
Stacks   : 1
Note     : 
Updata   : 2013-06-18
***********************************************************************/
void Init_IO()
{
   //     P3M0 = 0x00;
	//P3M1 = 0x00;
	P1 = 0xff;         //��LED�������ʾ
	KEY = 1;		
}
	
/***********************************************************************
Function : Init_TMR0
Input    : 
Output   : 
Return   : 
Stacks   : 1
Note     : ��ʼ����ʱ��������10ms�ж�
Updata   : 2013-06-18
***********************************************************************/
void Init_TMR0()
{
	TMOD = 0x21;
	IE |= 0x82;                            
	TL0 = 0Xff;
	TH0 = 0Xb7;
	TR0 = 1;
}

/***********************************************************************
Function : INTT0 interrupt
Input    : 
Output   : 
Return   : 
Stacks   : 1
Note     : ��ʱ���жϴ�����
Updata   : 2013-06-18
***********************************************************************/
void INTT0(void) interrupt 1 using 1
{
	TL0=0Xff;    // 10ms ��װ
	TH0=0Xb7;
	
        UpdateTimers();
}

/***********************************************************************
Function : Task0_led
Input    : 
Output   : 
Return   : 
Stacks   : 
Note     : ����һ��
           LED1���Ž��ڷ���ܸ�����
           LED1=0 Ϊ����LED1=1Ϊ��
Updata   : 2012-12-04
***********************************************************************/
unsigned char Task0_led()
{
    _SS
	while(1)
	{
	    LED1 = !LED1;
	    if (LED1) 
		{WaitX(45);}
	    else WaitX(5);
	}
    _EE
}

/************************************************
Function : Task0_key
Input    : 
Output   : 
Return   : 
Stacks   : 
Note     : �����
Updata   : 2012-12-04
************************************************/
unsigned char Task1_key()
{
    static bit SameKey_F = 0;

   _SS
	while(1)
	{
        if(KEY) 
        {
            if((++keycount >= 20)&&(!SameKey_F))
            { 
                IAP_CONTR = 0x60;
		SameKey_F = 1;
               
            }    
        }
        else
        {
            keycount = 0;
	    SameKey_F = 0;
        }
	//Ints_event_send(Task2_ID);
	SendSem(task_Wait_flag);
        WaitX(5);	 
	}
      _EE
}


/************************************************
Function : subtask
Input    : 
Output   : 
Return   : 
Stacks   : 
Note     : ����һ����������
Updata   : 2012-12-04
************************************************/
unsigned char  subtask()
{
    static char i;
    
    _SS
    while(1)
    {
        for(i=0;i<=5;i++)
	{
	P1=numtab[i];
        //Task_event_wait(wait_ever);
	WaitSem(task_Wait_flag);
        }
	}
    _EE
}


/************************************************
Function : Task2_display
Input    : 
Output   : 
Return   : 
Stacks   : 
Note     : �����������̡߳�д��,����������������
           ��ʽ��д���ȴ�ͳ״̬��д�����򵥣�
           ����Ȼ�����Ҹ�ʡROM��
           ����������κεط������Ե���������
Updata   : 2012-12-04
************************************************/
unsigned char  Task2_display()
{
    static char i;
    
    _SS 
    while(1)
    {
        for(i=0;i<=9;i++)                                                               //�ȴ�0--9������ʾ�����200mS
        {
       	P1=numtab[i];
        WaitX(20);
        }
        for(i=0;i<=9;i++)                                                               //Ȼ���0--9������ʾ�����500mS
        {
       	P1=numtab[i];
//Task_event_wait(50);
        WaitX(50);
	//if(task_Wait_flag) { Task_subCall(subtask); }
	if(task_Wait_flag)   { CallSub(subtask); }
        }
    }
    _EE
}


void main()
{
        Init_IO();
	Init_TMR0();	  
	
	while(1)
	{
  	   WDTC(); 
           RunTaskA(Task0_led,Task0_ID);
	   RunTaskA(Task1_key,Task1_ID);
	   RunTaskA(Task2_display,Task2_ID);
  	}
}
