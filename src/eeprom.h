#ifndef EEPROM_H
#define EEPROM_H
 
#include "config.h"

extern unsigned char EROM_read_byte(unsigned int addr);              //��ָ����ַһ�ֽ�����
extern void  EROM_write_byte(unsigned int addr, unsigned char dat);  //��ָ����ַдһ�ֽ�����
extern void  EROM_erase_sector(unsigned char n);               //����ָ������
extern void  EROM_read_continuous(unsigned int addr, unsigned char READPointer *pr, unsigned char len);   //��������EROM���ݵ�ָ������
extern void  EROM_write_continuous(unsigned int addr, unsigned char WRITEPointer *pr, unsigned char len); //��������������д��EROM
extern unsigned int EROM_read_word(unsigned int addr);             //��ָ����ַһ������
extern void  EROM_write_word(unsigned int addr, unsigned int dat); //��ָ����ַдһ������

#endif



