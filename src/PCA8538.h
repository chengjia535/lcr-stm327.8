
#ifndef     PCA8538_H
#define     PCA8538_H


void DlyMs(int n);
void PCA8538_SPI_Init(void);
void PCA8538_WriteData(unsigned char XPointer, unsigned char YPointer, unsigned char Data);

void Clear_LCD_Dianzhen(void);
 
void lcd_cls(void); //���� 
void lcd_init(void);

void LCD_SetXY(unsigned char x,unsigned char y);

void lcd_goto1(unsigned char x); //��1��xλ
void lcd_goto2(unsigned char x); //��2��xλ
void lcd_goto3(unsigned char x); //��3��xλ
void lcd_goto4(unsigned char x); //��4��xλ

//ȡ��������
//unsigned char *getASC(unsigned char ASC);
//�ַ����
void lcd_putc(uchar d);
//�ִ����
void lcd_puts(uchar *s);

#endif
