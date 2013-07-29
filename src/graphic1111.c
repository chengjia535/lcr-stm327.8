
#include "dot.h"
#include "cms015.h"

#define LCD_WIDTH	220
#define LCD_HIGHT	176

#define BLACK           0
#define RED             1
#define GREEN           2
#define BLUE            3
#define YELLOW          4

#define	DrPoint(c) WriteDAT(color[c][0],color[c][1]) 
//#define	DrPoint(BLACK) WriteDAT(color[BLACK][0],color[BLACK][1]) 

unsigned char buf[100];

unsigned char BACKCOLOR=0;

unsigned char color[16][2]={
{0x00,0x00},			//0-��
{0xf8,0x00},			//1-��
{0x07,0xe0},			//2-��
{0x00,0x1f},			//3-��
{0xff,0xe0},			//4-rg��
{0x07,0xff},			//5-gb
{0xf8,0x1f},			//6-rb��
{0x7b,0xef},			//7-��
{0x39,0xe7},			//8-ǳ��
{0x78,0x00},			//9-��
{0x03,0xe0},			//10-��
{0x00,0x0f},			//11-��
{0x7b,0xe0},			//12-rg��
{0x03,0xef},			//13-gb
{0x78,0x0f},			//14-rb��
{0xff,0xff}			//15-��
};



void set(unsigned int x,unsigned int y,unsigned char c)	//��һ����  
{	LCD_SetXY(x,y);
	DrPoint(c);
}

void h_line(unsigned int x1,unsigned int y,unsigned int x2,unsigned char c)
{	unsigned int i;
	for (i=x1;i<=x2;i++) {	set(i,y,c); }
}

void v_line(unsigned int x,unsigned int y1,unsigned int y2,unsigned char c)
{	unsigned int i;
	LCD_SetXY(x,y1);
	for (i=y1;i<=y2;i++) {	DrPoint(c); }	

}

void box(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned char c)
{	h_line(x1,y1,x2,c);
	v_line(x2,y1,y2,c);
	h_line(x1,y2,x2,c);
	v_line(x1,y1,y2,c);
}

void ClearLCD(unsigned char c)
{	unsigned int i;
	BACKCOLOR=c;
	LCD_SetXY(0,0);
	for(i=0;i<38720;i++)
	{ DrPoint(c); }
}

void boxfill(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned char c)
{	unsigned int i;
	for (i=x1;i<x2;i++)
	{	v_line(i,y1,y2,c);	}
}




void D57A(unsigned int x,unsigned int y,unsigned char n,unsigned char c)	//���һ��5*7�ַ�
{	unsigned char i,j,v;
	unsigned int nn;
	n=n-32;
	nn=n*5;
	for (i=0;i<5;i++)
	{  	v=data1[nn+i];
		LCD_SetXY(x+i,y);
		for (j=0;j<8;j++)
		{	if (v&1) {  DrPoint(c); }
			else DrPoint(BACKCOLOR);
			v=v>>1;
		}	
	}
}

void LCD_WDA57(unsigned int x, unsigned int y,unsigned char *string,unsigned char c) //x:x���� y:y���� dd:�ַ�
{
	unsigned int i=0;  
	unsigned int j=0;   
  
 	while(j==0)     
    {	D57A(x,y,string[i],c);     //˳����ʾ�ַ� 
      	i++;
	  	if (string[i]==0) j=1;                       
	  	x=x+6; 
    }     
}

void D12HZ(unsigned int x,unsigned int y,unsigned char n,unsigned char c)	//���һ��12������
{	unsigned char i,j,v;
	n=n-0x80;
	for (i=0;i<12;i++)
	{  	v=hz12[n][i];
		LCD_SetXY(x+i,y);
		for (j=0;j<8;j++)
		{	if (v&1)
			{  DrPoint(c); }
			else DrPoint(BACKCOLOR);
			v=v>>1;
		}
		v=hz12[n][i+12];
		for (j=0;j<4;j++)
		{	if (v&1) {  DrPoint(c); }
			else DrPoint(BACKCOLOR);
			v=v>>1;
		}	
	}
}

void LCD_WHZ12(unsigned int x, unsigned int y,unsigned char *string,unsigned char c) //x:x���� y:y���� dd:�ַ�
{
	unsigned char i=0;  
	unsigned char j=0;   
  
 	while(1)     
    {	j=string[i];
		if (j>=0x80)
		{	D12HZ(x,y,j,c);     //˳����ʾ�ַ� 
			x=x+12; 
		}
		else
		{	D57A(x,y+4,j,c);
			x=x+6;	
		}
      	i++;
	  	if (string[i]==0) return;
	  	
    }     
	
}

void D48ASC(unsigned int x,unsigned int y,unsigned char n,unsigned char c)	//���һ��47��������
{	unsigned char i,j,k,v;
	if (n==0x2e) n=0x3a;
	if (n==0x20) n=0x3d;
	n=n-0x30;
	for (i=0;i<24;i++)
	{  	LCD_SetXY(x+i,y);
		for (k=0;k<6;k++)
		{	v=asc48[n][i+k*24];
			for (j=0;j<8;j++)
			{	if (v&1)
				{  DrPoint(c); }
				else DrPoint(BACKCOLOR);
				v=v>>1;
			}
		}
	}
}

void LCD_WHZ48(unsigned int x, unsigned int y,unsigned char *string,unsigned char c) //x:x���� y:y���� dd:�ַ�
{
	unsigned char i=0;  
	unsigned char j=0;   
  
 	while(1)     
    {	j=string[i];
	D48ASC(x,y,j,c);
	x=x+24;	
      	i++;
	if (string[i]==0) return;
	  	
    }     
	
}

