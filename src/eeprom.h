#ifndef EEPROM_H
#define EEPROM_H
 
#include "config.h"

//������������ָ��ָ���������ͣ����㸴�ƣ�idata��xdata��code����:ͨ��ָ�룩
#define  READPointer          
//����д������ָ��ָ���������ͣ����㸴�ƣ�idata��xdata��code����:ͨ��ָ�룩
#define  WRITEPointer   

//��ָ����ַһ�ֽ�����
extern unsigned char EROM_read_byte(unsigned int addr);              

//��ָ����ַдһ�ֽ�����
extern void  EROM_write_byte(unsigned int addr, unsigned char dat);  

//����ָ������
extern void  EROM_erase_sector(unsigned char n);               

//��������EROM���ݵ�ָ������
extern void  EROM_read_continuous(unsigned int addr, unsigned char READPointer *pr, unsigned char len);   

//��������������д��EROM
extern void  EROM_write_continuous(unsigned int addr, unsigned char WRITEPointer *pr, unsigned char len); 

//��ָ����ַһ������
extern unsigned int EROM_read_word(unsigned int addr);             

//��ָ����ַдһ������
extern void  EROM_write_word(unsigned int addr, unsigned int dat); 

//��ڶ�����д����
void cs_RW(unsigned char rw); 

#endif

