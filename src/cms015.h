
#ifndef   CMS015_H 
#define   CMS015_H 

void LCD_INIT(void);
void WriteDAT8(unsigned char b);     //д8λ����
void WriteCOM(u8 b);
void WriteDAT(u8 a,u8 b); //д16λ����
void LCD_CMD(u16 cmd,u16 dat);
//void LCD_SetWin(u8 x1,u8 y1,u8 x2,u8 y2);	//������ʾ����
void LCD_SetXY(u8 x,u8 y);
//void LCD_ReSetWin(void);	//�ָ�ԭʼ����

#endif
