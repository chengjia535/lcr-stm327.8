#include <STC89C51.h>

/**************СС��������ʼ�� *********************************/
#define MAXTASKS 3	    //���������������ɰ������ӣ����Ϊ255����
char  timers[MAXTASKS];
char currid; //��ǰ���е������
#define _SS   static char lc=0; switch(lc){   case 0: 	 //��ת��ʼ
#define _EE   }; lc=0;  //��ת����
#define DelayX(b)  settimer(&lc,__LINE__,b); return ; case __LINE__: //��������ʱ�ȴ���������
#define RunTask(a,b) currid=b; if (timers[b]==0){timers[b]=-1; a();} //���ж�������
#define CallSub(x)  DelayX(0);x(); if (timers[currid]!=-1) return;  	 //����������
void settimer(char *lc,char  line,char d){//���ö�ʱ��
  *lc=line;  timers[currid]=d;
}
/****************СС������������*********************************/

sbit KEY = P3^2;
unsigned char code numtab[16]={0x24,0x6F,0xE0,0x62,0x2B,0x32,0x30,0x67,0x20,0x22,0x21,0x38,0xB4,0x68,0xB0,0xB1};

sfr IAP_CONTR = 0xC7; 
sfr WDT_CONTR = 0xC1;
//������Ź�
void clr_wdt()
{
  WDT_CONTR =0x3C; 
}
//��ʼ����ʱ��������10ms�ж�
void InitT0()
{
	TMOD = 0x21;
	IE |= 0x82;  // 12t
	TL0=0Xff;
	TH0=0Xb7;
	TR0 = 1;
}
//��ʱ���жϴ�����
void INTT0(void) interrupt 1 using 1
{
    unsigned char i;
	TL0=0Xff;    //10ms ��װ
	TH0=0Xb7;
	//�߼���ʱ������
    for (i=0;i<MAXTASKS;i++){
     if (timers[i]>0) {
	   timers[i]--;
	 }
    }	
}

sbit LED1= P2^4;  

//����һ��״̬��д��
void ontimer0(){
  LED1=!LED1;	// LED1���Ž��ڷ���ܸ�����LED1=0 Ϊ����LED1=1Ϊ��  
  if (LED1) timers[currid]=45;	//450mS ��
  else timers[currid]=5;	//50ms  ��
}

//�������״̬��д��
char keycount=0;
void task1(){
 if(KEY==0) {
   keycount++;
   if (keycount>20) IAP_CONTR = 0x60;
 }
 else{
    keycount=0;
 }
 timers[currid]=5;	 //��װ��ʱ��
}

//����һ����������
void  subtask()
{
 static char i;
_SS
 for(i=0;i<=5;i++){
  DelayX(20);
  P1=numtab[i];
 }
_EE
}

//�����������̡߳�д�������������������ַ�ʽ��д���ȴ�ͳ״̬��д�����򵥣�����Ȼ�����Ҹ�ʡROM��
void  task2()
{
 static char i;	 //����������Ϊ��̬�ġ�
_SS

while(1){
 for(i=0;i<=9;i++){	  //�ȴ�0--9������ʾ�����200mS
  DelayX(20);
  P1=numtab[i];
 }
 for(i=0;i<=9;i++){//Ȼ���0--9������ʾ�����500mS
  DelayX(50);
  P1=numtab[i];
  CallSub(subtask); //����������κεط������Ե���������
 }
  //CallSub(subtask); //����������κεط������Ե���������
}

_EE
}

void main()
{
	P3M0 = 0x00;
	P3M1 =0x00;
	P1 = 0xff;         //��LED�������ʾ

	InitT0();	  //��ʼ����ʱ�����Ա����10ms�ж�
	KEY =1;		  //����IO��	
    
	while(1){
  	   clr_wdt(); //������Ź�

	   //���ֻ�����ж������񼴿�
	   RunTask(ontimer0,0);	//ontimer0������ڵ�0�������
	   RunTask(task1,1);	//task1������ڵ�1�������
	   RunTask(task2,2);    //task2������ڵ�2�������
	   //...���������Ӹ�������
  	}
}
