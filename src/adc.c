
#include <reg52.h>
#include <intrins.h>  
#include "delay.h"

//ADת��
sfr P1ASF = 0x9D;     //��P1��Ϊģ��ڼĴ���(ʹ��),��λ��Ϊ1����Ч
sfr ADC_CONTR = 0xBC; //A/Dת�����ƼĴ���
sfr ADC_RES   = 0xBD; //A/Dת������Ĵ���
sfr ADC_RESL  = 0xBE; //A/Dת������Ĵ���

/***********************************************************************
Function : ADC_Init 
Note     : ADC��ʼ��
***********************************************************************/
void ADC_Init(unsigned char Channel){  
	P1ASF = 0x01<<Channel;  //ѡ��ADC��8���ӿ��е�һ������ӦλΪ1ѡ�У�  
        ADC_CONTR = 0x40;       //ADCת�����ٶ�140��ʱ�����ڣ�0XX0 0000 ����XX�����ٶȣ�  
        _nop_();  
        ADC_CONTR |= Channel;       //ѡ��A/D��ǰͨ��  
        _nop_();  
        ADC_CONTR |= 0x80;      //����A/D��Դ  
        DelayMs(1);             //ʹ�����ѹ�ﵽ�ȶ���1ms���ɣ�  
    }  
      
/***********************************************************************
Function : ADC_Read
Note     : ��ADCֵ
***********************************************************************/
unsigned int ADC_Read (void){  
        ADC_CONTR |= 0x08;      //����A/Dת����0000 1000 ��ADCS = 1��  
        _nop_();  
        _nop_();  
        _nop_();  
        _nop_();  
	while ( !(ADC_CONTR & 0x10) ); //�ȴ�A/Dת������(ADC_FLAG==0)
        ADC_CONTR &= 0xE7;      //1111 0111 ��ADC_FLAGλ, �ر�A/Dת��,   
   return (ADC_RES*4+ADC_RESL);//����A/Dת�������10λADC���ݸ�8λ��ADC_RES�У���2λ��ADC_RESL�У�  
    }  





 
