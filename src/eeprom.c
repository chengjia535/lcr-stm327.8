
#include <intrins.h> 
#include "config.h"
 
//�������Ƿ�ʹ���ж�
#define  USE_ISR       1   

#define  USE_EROM_read_continuous     0
#define  USE_EROM_write_continuous    0
#define  USE_EROM_read_word           0
#define  USE_EROM_write_word          0
         
// STC89C52RC         EEOPROM��С�� 4K����ַ��0x2000��������0x5A��0xA5 
// STC11F01E          EEOPROM��С�� 2K����ַ��0x0000��������0x5A��0xA5 
// STC12C5410AD       EEOPROM��С��12K����ַ��0x0000��������0x46��0xB9 
// STC12C4052         EEOPROM��С��10K����ַ��0x0000��������0x46��0xB9
// STC12C5A32S2       EEOPROM��С��28K����ַ��0x0000��������0x5A��0xA5
// STC12C5A60S2       EEOPROM��С�� 1K����ַ��0x0000��������0x5A��0xA5
#define  STC12C5A60S2
// STC15F104E         EEOPROM��С�� 1K����ַ��0x0000��������0x5A��0xA5
// STC15F204EA        EEOPROM��С�� 1K����ַ��0x0000��������0x5A��0xA5

#if defined (__STC10xxx_H__)\
  || defined (__STC11xxx_H__)\ 
  || defined (__STC12xx052_H__)\
  || defined (__STC12x52xx_H__)\
  || defined (__STC12x54xx_H__)\
  || defined (__STC12x56xx_H__)\
  || defined (STC12C5A60S2)\
  || defined (__STC15F104E_H__)\
  || defined (__STC15F204EA_H__)  
           #define  EROM_START    0x0000 
#elif defined (__STC90x51_H__)
           #define  EROM_START    0x1000
#elif defined (__STC89x51_H__) || defined (__STC89x52_H__) || defined (__STC90x52_H__) 
           #define  EROM_START    0x2000
#elif defined (__STC90x12_H__)
           #define  EROM_START    0x3000
#elif defined (__STC89x54_H__) || defined (__STC90x54_H__) 
           #define  EROM_START    0x4000
#elif defined (__STC89x58_H__) || defined (__STC90x58_H__)
           #define  EROM_START    0x8000
#elif defined (__STC89x510_H__) || defined (__STC90x510_H__)
           #define  EROM_START    0xA000        
#elif defined (__STC89x512_H__) || defined (__STC90x512_H__)
           #define  EROM_START    0xC000
#elif defined (__STC89x514_H__) || defined (__STC90x514_H__)
           #define  EROM_START    0xE000        
#elif
     #warning "STC�ͺ�δ���壬��Ӧ�궨�� EROM_START ȱʧ"  
#endif
 
//ISP���������ͬSTC�ͺ�������ͬ��
#if defined (__STC10xxx_H__)\
  || defined (__STC11xxx_H__)\ 
  || defined (__STC12x52xx_H__)\
  || defined (STC12C5A60S2)\
  || defined (__STC15F104E_H__)\
  || defined (__STC15F204EA_H__) //STC10xxx, STC11xxx, STC12x5Axx, 12x52xxϵ��ISP��������: 0x5A��0xA5      
     #define  ISPTRIG()           ISP_TRIG = 0x5A, ISP_TRIG = 0xA5                
#elif defined (__STC89_90xxx_H__)\ 
  || defined (__STC12xx052_H__)\
  || defined (__STC12x54xx_H__)\
  || defined (__STC12x56xx_H__) //STC89xxx, STC90xxx, STC12xx052, STC12x54xx, STC12x56xxϵ��ISP��������: 0x46��0xB9      
     #define  ISPTRIG()           ISP_TRIG = 0x46, ISP_TRIG = 0xB9
#elif
      #warning "STC�ͺ�δ���壬��Ӧ�궨�� ISPTRIG() ȱʧ"         
#endif        
 
 
#if   ( CPU_FOSC >= 24000000 )
     #define  EN_ISP  0x80          //ϵͳʱ�� >= 24MHz
#elif ( CPU_FOSC >= 20000000 )
     #define  EN_ISP  0x81          //ϵͳʱ�� >= 20MHz
#elif ( CPU_FOSC >= 12000000 )
     #define  EN_ISP  0x82          //ϵͳʱ�� >= 12MHz
#elif ( CPU_FOSC >=  6000000 )
     #define  EN_ISP  0x83          //ϵͳʱ�� >= 6MHz
#elif ( CPU_FOSC >=  3000000 )
     #define  EN_ISP  0x84          //ϵͳʱ�� >= 3MHz
#elif ( CPU_FOSC >=  2000000 )
     #define  EN_ISP  0x85          //ϵͳʱ�� >= 2MHz
#elif ( CPU_FOSC >=  1000000 )
     #define  EN_ISP  0x86          //ϵͳʱ�� >= 1MHz
#else
     #define  EN_ISP  0x87          //ϵͳʱ�� < 1MHz
#endif
         

extern cs;
                                 
/********************************************************************
������IAP�������� + ��ֹIAP��������IAP��ؼĴ�����
********************************************************************/
static void command_and_close( void )
 {
 #if (USE_ISR)  
         EA = 0;   
 #endif
 
     ISPTRIG();                
     _nop_(); 
     _nop_(); 
     _nop_();
  
 #if (USE_ISR)
         EA = 1;  
 #endif
           
     ISP_CONTR = 0;    
     ISP_CMD   = 0;          
     ISP_TRIG  = 0;                   
     ISP_ADDRH = 0xFF;      
     ISP_ADDRL = 0xFF;              
 }
 
/********************************************************************
 ��������ָ����ַһ�ֽ�����                   
 ���룺����ַ�� 
 ���أ�����ַ���ݣ� 
********************************************************************/
unsigned char EROM_read_byte(unsigned int addr)
 {                      
     ISP_CONTR = EN_ISP;   
     ISP_CMD   = 1;                   //�����ȡEEPROM
     ISP_ADDRL = addr;                
     ISP_ADDRH = (unsigned char)(addr>>8) + (unsigned char)(EROM_START>>8);               
     command_and_close();
           
     return ISP_DATA;                        
 }
 
/********************************************************************
 ��������ָ����ַдһ�ֽ�����
 ���룺����ַ�����ݣ�
********************************************************************/
void EROM_write_byte(unsigned int addr, unsigned char dat)
 {     
     ISP_CONTR = EN_ISP;        
     ISP_CMD   = 2;                  //�����дEEPROM
     ISP_ADDRL = addr;                
     ISP_ADDRH = (unsigned char)(addr>>8) + (unsigned char)(EROM_START>>8);           
     ISP_DATA  = dat;
                                   
     command_and_close();    
 }
 
/********************************************************************
 ����������ָ������          
 ���룺�������ţ�
********************************************************************/
void EROM_erase_sector(unsigned char n)
 {           
     ISP_CONTR = EN_ISP; 
     ISP_CMD   = 3;            //�������EEPROM     
     ISP_ADDRL = 0x00;                
     ISP_ADDRH = (unsigned char)(EROM_START>>8) + n*0x02; 
                                            
     command_and_close();
 }
 
/********************************************************************
 ����������������                   
 ���룺����ʼ��ַ��ָ��ָ���ֽڳ���
********************************************************************/
#if (USE_EROM_read_continuous)
void EROM_read_continuous(unsigned int addr, unsigned char READPointer *pr, unsigned char len)
 {                         
     while (len--)
     { 
         *pr = EROM_read_byte(addr);
         pr++;
         addr++;
     }                                    
 }
#endif
 
/********************************************************************
 ����������д����
 ���룺����ʼ��ַ��ָ��ָ���ֽڳ���
********************************************************************/
#if (USE_EROM_write_continuous)
void EROM_write_continuous(unsigned int addr, unsigned char WRITEPointer *pr, unsigned char len)
 {
     while (len--)
     { 
         EROM_write_byte(addr, *pr); 
         pr++;
         addr++;
     }                  
 }        
#endif
 
/********************************************************************
 ��������ָ����ַһ������        
********************************************************************/
#if (USE_EROM_read_word)
unsigned int EROM_read_word(unsigned int addr)
 {                      
     unsigned int dat;
   
     ((unsigned char *)&dat)[0] = EROM_read_byte(addr++);
     ((unsigned char *)&dat)[1] = EROM_read_byte(addr);
     return dat;                        
 }
#endif
 
/********************************************************************
 ��������ָ����ַдһ������
********************************************************************/
#if (USE_EROM_write_word)
void EROM_write_word(unsigned int addr, unsigned int dat)
 {     
     EROM_write_byte(addr++, ((unsigned char *)&dat)[0]); 
     EROM_write_byte(addr, ((unsigned char *)&dat)[1]);   
 }
#endif

/********************************************************************
 ��������ڶ�������д����
********************************************************************/
void cs_RW(unsigned char rw)
{
 unsigned char i,*p = &cs;
 const unsigned int offs=512;
 if(rw){
  EROM_erase_sector(2); //�ڶ�����
  for(i=0;i<sizeof(cs);i++) EROM_write_byte(i+offs,p[i]);
 }else{
  for(i=0;i<sizeof(cs);i++) p[i]=EROM_read_byte(i+offs);
 }
}


