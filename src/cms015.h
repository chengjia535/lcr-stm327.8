
#ifndef   CMS015_H 
#define   CMS015_H 

void LCD_INIT(void);
void WriteDAT8(unsigned char b);     //д8λ����
void WriteCMD(unsigned char b);
void WriteDAT(unsigned char a,unsigned char b); //д16λ����
void LCD_CMD(unsigned int cmd,unsigned int dat);
//void LCD_SetWin(u8 x1,u8 y1,u8 x2,u8 y2);	//������ʾ����
void LCD_SetXY(unsigned char x,unsigned char y);
//void LCD_ReSetWin(void);	//�ָ�ԭʼ����

#endif
