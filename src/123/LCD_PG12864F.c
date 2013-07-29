#include "nx_C51.h"
#include "LCD_PG12864F.h"
#include "T6963.h"

// use external memory but to operate the commands and datas
static volatile BYTE xdata lcd_cmd _at_ LCD_ADDRESS_CMD;
static volatile BYTE xdata lcd_data _at_ LCD_ADDRESS_DAT;




//////////////////////////////////////////////////////////////////////////
//����,xΪ��, yΪ�ַ���
//void LCD_MoveCursorToChar(BYTE x, BYTE yCh)

//����,xΪ�ַ��±�, yΪ��߶�
void LCD_MoveCursorToChar(BYTE xCh, BYTE y)
{
    WORD add;
    
    add = y * _SCREEN_X_CHARS + xCh;
    
    //���õ�ַ
    T6963_WR(_L_OF_WORD(add));
    T6963_WR(_H_OF_WORD(add));
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);    
}


//////////////////////////////////////////////////////////////////////////
//  ����ӳ���
//////////////////////////////////////////////////////////////////////////
void LCD_Point(BYTE Px, BYTE Py, BYTE mode) 
{
    BYTE tempD;
    
    if(Px >= _SCREEN_X_DOTS || Py >= _SCREEN_Y_DOTS)
    {
        return;
    }
        
    LCD_MoveCursorToChar(Px >> 3, Py);

    if(mode == _GUI_MODE_CVR || mode == _GUI_MODE_OR)
    {
        T6963_CMD(_CMD_BIT_SET(7 - (Px & 0x07)));
    }
    else if(mode == _GUI_MODE_FLP)
    {
        tempD = T6963_ReadData(No_move);
        _FLP_BIT(tempD, 7 - (Px & 0x07));
        T6963_WriteData(tempD, No_move);
    }
}



//////////////////////////////////////////////////////////////////////////
// ���ܣ��Ը��ǵķ�ʽ��ˮƽ�ߡ�
// ��ڲ�����x0		ˮƽ����������е�λ��
//           y0		ˮƽ����������е�λ��
//           x1          ˮƽ���յ������е�λ��
//           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
// ���ڲ�������
// ˵��������ʧ��ԭ����ָ����ַ������������Χ��
//////////////////////////////////////////////////////////////////////////
void LCD_HLineCVR(BYTE x0, BYTE x1, BYTE ra0, BYTE ra1)
{
    BYTE dataA, i;

    if(x0 == x1)                                //��ֹ����ͬһ������
    {
        i = T6963_ReadData(No_move);        //����һ������
      
        dataA = (0xFF >> ra0) & (0xFF << 8 - ra1);//���ݿ������
        i |= dataA;          
            
        T6963_WriteData(i, Increase);        
    }
    else
    {
        i = T6963_ReadData(No_move);        //��һ������    
        dataA = i | (0xFF >> ra0);          //��һ���ǰ�벿�ִ����ж���
        T6963_WriteData(dataA, Increase);
            
        for(i = x0 + 1; i < x1; ++i)
        {
            T6963_WriteData(0xFF, Increase);
        }

        i = T6963_ReadData(No_move);        //���һ������                
        dataA = i | ~(0xFF >> ra1);      
        T6963_WriteData(dataA, Increase);
    }
}

//////////////////////////////////////////////////////////////////////////
// ���ܣ���ˮƽ�ߡ�
// ��ڲ�����x0		ˮƽ����������е�λ��
//           y0		ˮƽ����������е�λ��
//           x1          ˮƽ���յ������е�λ��
//           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
// ���ڲ�������
// ˵��������ʧ��ԭ����ָ����ַ������������Χ��
//////////////////////////////////////////////////////////////////////////
void LCD_HLine(BYTE x0, BYTE y0, BYTE x1, BYTE mode)
{
    BYTE ra0, ra1;
    
    if(x0 > x1) 				// ��x0��x1��С�������У��Ա㻭ͼ
    {
        _SWAP(x0, x1);
    }

    x1++;                       //ת��Ϊ�������
    
    ra0 = x0 & 0x07;
    ra1 = x1 & 0x07;
    x0 >>= 3;
    x1 >>= 3;
    
    LCD_MoveCursorToChar(x0, y0);
    
    if(mode == _GUI_MODE_CVR || mode == _GUI_MODE_OR)
    {
        LCD_HLineCVR(x0, x1, ra0, ra1);
    }
    else
    {
    }    
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void LCD_VLine(BYTE x0, BYTE y0, BYTE y1, BYTE mode)
{
    if(y0 > y1)                             // ��y0��y1��С�������У��Ա㻭ͼ
    {
        _SWAP(y0, y1);
    }
    
    y1++;                                   //ת��Ϊ�������
    
    for(; y0 < y1; ++y0)
    {
        LCD_Point(x0, y0, mode);	                // �����ʾ�������ֱ��
    }
}


//////////////////////////////////////////////////////////////////////////
void LCD_Rectangle(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    LCD_HLine(x0, y0, x1, mode);
    LCD_VLine(x1, y0, y1, mode);
    LCD_VLine(x0, y0, y1, mode);
    LCD_HLine(x0, y1, x1, mode);
}


//////////////////////////////////////////////////////////////////////////
// ���ƣ�LCD_RectangleFill()
// ���ܣ������Ρ���һ�����ľ��Σ����ɫ��߿�ɫһ����
// ��ڲ�����x0		�������Ͻǵ�x����ֵ
//           y0		�������Ͻǵ�y����ֵ
//           x1          �������½ǵ�x����ֵ
//           y1          �������½ǵ�y����ֵ
//           color	����ַ�
// ���ڲ�������
// ˵��������ʧ��ԭ����ָ����ַ������Ч��Χ��
//////////////////////////////////////////////////////////////////////////
void LCD_RectangleFill(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    BYTE i;

    if(x0 > x1)
    {
        _SWAP(x1, x0);
    }
    if(y0 > y1)
    {
        _SWAP(y1, y0);
    }
    
    if(x0 == x1 || y0 == y1 || x0 >= _SCREEN_X_DOTS || y0 >= _SCREEN_Y_DOTS)
    {
        return;
    }

    if(x1 > _SCREEN_X_DOTS)
    {
        x1 = _SCREEN_X_DOTS;
    }
    
    if(y1 > _SCREEN_Y_DOTS)
    {
        y1 = _SCREEN_Y_DOTS;
    }
        
    //����
    for(i = y0; i <= y1; ++i)
    {
        LCD_HLine(x0, i, x1, mode);
    }

    return;
}




//�������ֽ���䷽ʽд��һ��ָ���������ֽڻ�����
//pFont: Ҫ�����ֽ�����ָ��
//bytes: �������ģ���ֽ���,����һ�ֽڵİ�һ�ֽ���, ������������ģ�Ĵ�С, ���ǵ�ǰ�л��еĴ�С
//pos0 : Ҫ������ʼλ��(����Ļ�ϵ����ֽ�Ϊ��λ��λ��, ����Ϊ��, ����Ϊ��)
//pos1 : Ҫ���Ľ���λ��
//ra0  : ��ʼ���������ֽڵ�ƫ����(����), ����:| _ _ _ x x x x x | ƫ����Ϊ3
//ra1  : �������������ֽڵ�ƫ����(����), ����:| x x x x x _ _ _ | ƫ����Ϊ5 
void LCD_ShowStreamCVR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //��ֹ����ͬһ������
    {
        stuff = T6963_ReadData(No_move);        //����һ������
      
        dataB = (~(0xFF >> ra0)) | (0xFF >> ra1);//���ݿ������
        dataB &= stuff;                         //ȥ�����ϴ���䲿�ֵ�����
        dataA = (*pFont >> ra0) | dataB;        //��ģ�����������
            
        T6963_WriteData(dataA, Increase);        
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i < pos1; ++i)
                {
                    T6963_WriteData(*pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    T6963_WriteData(*pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//���һ������
        
                dataA = stuff & (0xFF >> ra1);
                dataA |= *pFont;
                T6963_WriteData(dataA, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //��һ������
    
            dataB = stuff & (~(0xFF >> ra0));       //��һ���ǰ�벿�ִ����ж���
            dataA = (*pFont >> ra0) | dataB;        //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
            dataB = (*pFont++) << (8 - ra0);        //��ģ���ݺ�벿��(�����������ģ�հ������0)
        
            T6963_WriteData(dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                //�ƶ���ģ
                dataA = (*pFont >> ra0) | dataB;        //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                dataB = (*pFont++) << (8 - ra0);        //��ģ���ݺ�벿��(�����������ģ�հ������0)

                T6963_WriteData(dataA, Increase);
            }

            if(ra1 != 0)
            {
                stuff = T6963_ReadData(No_move);        //���һ������
                
                if(bytes <= 1 || ra0 == ra1)            //�����Ϊ���ֽ�ʱ, ra0 == ra1
                {
                    dataA = stuff & (0xFF >> ra1) | dataB;
                }
                else
                {
                    dataA = (*pFont >> ra0) | dataB;
                    dataB = stuff & (0xFF >> ra1);
                    dataA = dataB | dataA;
                }
        
                T6963_WriteData(dataA, Increase);
            }
        }
    }
}

//�������ֽ���䷽ʽд��һ��ָ���������ֽڻ�����, ��ԭ���ݽ���"���"����
//pFont: Ҫ�����ֽ�����ָ��
//bytes: �������ģ���ֽ���,����һ�ֽڵİ�һ�ֽ���, ������������ģ�Ĵ�С, ���ǵ�ǰ�л��еĴ�С
//pos0 : Ҫ������ʼλ��(����Ļ�ϵ����ֽ�Ϊ��λ��λ��, ����Ϊ��, ����Ϊ��)
//pos1 : Ҫ���Ľ���λ��
//ra0  : ��ʼ���������ֽڵ�ƫ����(����), ����:| _ _ _ x x x x x | ƫ����Ϊ3
//ra1  : �������������ֽڵ�ƫ����(����), ����:| x x x x x _ _ _ | ƫ����Ϊ5 
void ShowBMPOneStreamXOR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //��ֹ����ͬһ������
    {
        stuff = T6963_ReadData(No_move);        //����һ������
        dataA = (*pFont >> ra0) ^ stuff;        //��ģ������ԭ��������
        T6963_WriteData(dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff ^ *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff ^ *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//���һ������
                T6963_WriteData(stuff ^ *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //��һ������
    
            dataA = *pFont >> ra0;                  //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
            dataB = (*pFont++) << (8 - ra0);        //��ģ���ݺ�벿��(�����������ģ�հ������0)
        
            T6963_WriteData(stuff ^ dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //�ƶ���ģ
                dataA = *pFont >> ra0 | dataB;      //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                dataB = (*pFont++) << (8 - ra0);    //��ģ���ݺ�벿��(�����������ģ�հ������0)

                T6963_WriteData(stuff ^ dataA, Increase);
            }

            if(ra1 != 0)
            {
                stuff = T6963_ReadData(No_move);        //���һ������
                
                if(bytes <= 1 || ra0 == ra1)            //�����Ϊ���ֽ�ʱ, ra0 == ra1
                {
                    dataA = dataB;
                }
                else
                {
                    dataA = (*pFont >> ra0) | dataB;
                }
                
                T6963_WriteData(stuff ^ dataA, Increase);
            }
        }
    }
}

//�������ֽ���䷽ʽд��һ��ָ���������ֽڻ�����, ��ԭ���ݽ���"��"����
//pFont: Ҫ�����ֽ�����ָ��
//pos0 : Ҫ������ʼλ��(����Ļ�ϵ����ֽ�Ϊ��λ��λ��, ����Ϊ��, ����Ϊ��)
//pos1 : Ҫ���Ľ���λ��
//ra0  : ��ʼ���������ֽڵ�ƫ����(����), ����:| _ _ _ x x x x x | ƫ����Ϊ3
//ra1  : �������������ֽڵ�ƫ����(����), ����:| x x x x x _ _ _ | ƫ����Ϊ5 
void ShowBMPOneStreamAND(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //��ֹ����ͬһ������
    {
        stuff = T6963_ReadData(No_move);        //����һ������
        dataA = (*pFont >> ra0) | (~(0xFF >> ra0));  //��ģ������ԭ��������
                                                //ע��������֮ǰҪ������������Ϊ1
        T6963_WriteData(stuff & dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i <= pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff & *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff & *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//���һ������
                T6963_WriteData(stuff & *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //��һ������
    
            dataA = (*pFont >> ra0) | (~(0xFF >> ra0));
            dataB = (*pFont++) << (8 - ra0);        //��ģ���ݺ�벿��(�����������ģ�հ������0)
        
            T6963_WriteData(stuff & dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //�ƶ���ģ
                dataA = *pFont >> ra0 | dataB;      //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                dataB = (*pFont++) << (8 - ra0);    //��ģ���ݺ�벿��(�����������ģ�հ������0)

                T6963_WriteData(stuff & dataA, Increase);
            }

            stuff = T6963_ReadData(No_move);        //���һ������

            if(ra1 == 0)
            {
                dataA = (*pFont >> ra0) | dataB;    //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                T6963_WriteData(stuff & dataA, Increase);
            }                
            else
            {
                dataB |= 0xFF >> ra0;
                T6963_WriteData(stuff & dataB, Increase);
            }
        }
    }
}

//�������ֽ���䷽ʽд��һ��ָ���������ֽڻ�����, ��ԭ���ݽ���"��"����
//pFont: Ҫ�����ֽ�����ָ��
//pos0 : Ҫ������ʼλ��(����Ļ�ϵ����ֽ�Ϊ��λ��λ��, ����Ϊ��, ����Ϊ��)
//pos1 : Ҫ���Ľ���λ��
//ra0  : ��ʼ���������ֽڵ�ƫ����(����), ����:| _ _ _ x x x x x | ƫ����Ϊ3
//ra1  : �������������ֽڵ�ƫ����(����), ����:| x x x x x _ _ _ | ƫ����Ϊ5 
void ShowBMPOneStreamOR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //��ֹ����ͬһ������
    {
        stuff = T6963_ReadData(No_move);        //����һ������
        dataA = (*pFont >> ra0) | stuff;        //��ģ������ԭ��������
        T6963_WriteData(dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i <= pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff | *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff | *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//���һ������
                T6963_WriteData(stuff | *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //��һ������
    
            dataA = *pFont >> ra0;                  //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
            dataB = (*pFont++) << (8 - ra0);        //��ģ���ݺ�벿��(�����������ģ�հ������0)
        
            T6963_WriteData(stuff | dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //�ƶ���ģ
                dataA = *pFont >> ra0 | dataB;      //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                dataB = (*pFont++) << (8 - ra0);    //��ģ���ݺ�벿��(�����������ģ�հ������0)

                T6963_WriteData(stuff | dataA, Increase);
            }

            stuff = T6963_ReadData(No_move);        //���һ������

            if(ra1 == 0)
            {
                dataA = (*pFont >> ra0) | dataB;    //��ģ����ǰ�벿�����ϴεĺ�벿�ֺϲ�
                T6963_WriteData(stuff | dataA, Increase);
            }                
            else
            {
                T6963_WriteData(stuff | dataB, Increase);
            }
        }
    }
}




//////////////////////////////////////////////////////////////////////////
//��ָ��λ�ô�(x0, y0)��ʼ, ��ָ����С��(width, height)��ͼƬ(pFont),
//����ָ��ģʽ(mode: ��, ���, ��)
void LCD_ShowArray(const BYTE *pFont, BYTE x0, BYTE y0, BYTE width, BYTE height, BYTE mode)
{
    BYTE i;
    BYTE y1, x1, ra0, ra1, fntAdd;
    WORD temp;
    
    if(pFont == NULL || width == 0 || height == 0 || x0 >= _SCREEN_X_DOTS || y0 >= _SCREEN_Y_DOTS)
    {
        return;
    }

    temp = (WORD)x0 + (WORD)width;
    if(temp > _SCREEN_X_DOTS)
    {
        temp = _SCREEN_X_DOTS;
    }
    x1 = (BYTE)temp;
    
    temp = (WORD)y0 + (WORD)height;
    if(temp > _SCREEN_Y_DOTS)
    {
        temp = _SCREEN_Y_DOTS;
    }
    y1 = (BYTE)temp;               //ͼ���������½������ϵ�����
    
    //����
    //ra0 = y0 & 0x07;            //��ʼ�����ֽ��е�ƫ��, (����8�������)
    //ra1 = y1 & 0x07;            //��ֹ�����ֽ��е�ƫ��
    
    //����
    fntAdd = (width >> 3) + ((width & 7) > 0);//����ȵ�λת��Ϊ�ֽ�

    ra0 = x0 & 0x07;            //��ʼ�����ֽ��е�ƫ��, (����8�������)
    ra1 = x1 & 0x07;            //��ֹ�����ֽ��е�ƫ��

    x0 >>= 3;                   //����ת��Ϊ�洢��Ԫ����(����3λ������8)
    x1 >>= 3;
    
    if(mode == _GUI_MODE_CVR)
    {
        for(i = y0; i < y1; ++i)     //���д���
        {
            LCD_MoveCursorToChar(x0, i);
            LCD_ShowStreamCVR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                 
        }
    }
    else if(mode == _GUI_MODE_AND)
    {
        for(i = y0; i < y1; ++i)     //���д���
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamAND(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }
    else if(mode == _GUI_MODE_OR)
    {
        for(i = y0; i < y1; ++i)     //���д���
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamOR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }
    else if(mode == _GUI_MODE_FLP)
    {
        for(i = y0; i < y1; ++i)     //���д���
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamXOR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }

    return;
}

//////////////////////////////////////////////////////////////////////////
//���ߡ����ⷽ���б��,ֱ����ѧ���� aX+bY=1 
//////////////////////////////////////////////////////////////////////////
// ���������д��޸�
void LCD_Line(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    BYTE t;
    SWORD xerr, yerr, delta_x, delta_y, distance;
    SBYTE incx, incy;
    
    if(x0 == x1)
    {
        LCD_VLine(x0, y0, y1, mode);
        return;        
    } 
    
    if(y0 == y1)
    {
        LCD_HLine(x0, y0, x1, mode);
        return;
    }

    delta_x = x1 - x0;                 //������������
    delta_y = y1 - y0;
    
    if(delta_x > 0)
    {
        incx = 1;                       //���õ�������
    }
    else 
    {
        incx = -1;
        delta_x = -delta_x;
    }
    
    if(delta_y > 0)
    {
        incy = 1;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
    distance = (delta_x > delta_y)? delta_x:delta_y;//ѡȡ��������������

    xerr = 0, yerr = 0;
    for(t = 0; t <= distance + 1; t++)
    {                                         
        LCD_Point(x0, y0, mode);              //����
        
        xerr += delta_x;
        yerr += delta_y;
        
        if(xerr > distance)
        {
            xerr -= distance;
            x0 += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            y0 += incy;
        }
    }
}


///----------------------------------------------------------------------------
void LCD_CLS(void)
{
    BYTE i, j, k = 1;
    
    //���õ�ַ
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    //��ʼ�Զ�д��
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(j = 0; j < _SCREEN_Y_DOTS; j++)
    {
        for(i = 0; i < _SCREEN_X_CHARS; i++)
        {
            T6963_AutoWR(0x00);
        }
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);
}

//test
void LCD_Test(void)
{
    /*
    BYTE i, j, k;
    //���õ�ַ
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    T6963_WR(0xF0);
    //��ʼ�Զ�д��
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(j = 0; j < _SCREEN_Y_DOTS; j++)
    {
        for(i = 0; i < _SCREEN_X_CHARS; i++)
        {
            T6963_AutoWR(i);
        }
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);*/
}

//���RAM����(������Ļ������)
void LCD_ClearRAM(void)
{
    WORD i;
    
    //���õ�ַ
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    //��ʼ�Զ�д��
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(i = 0x0000; i < 0x2000; i++)            //8K ȫ�����
    {
        T6963_AutoWR(0x00);
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);
}


//��ʼ��
void LCD_Init(void)
{
    T6963_InitBus();
    
    LCD_RST_EN;
    Delay1(200);
    LCD_RST_UN;

    //ͼ����ʼ��ַ
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_CTRL | _CMD_CTRL_GRAPHIC_HOME);

    //ͼ������
    T6963_WR(_SCREEN_X_CHARS);   //240����, ����8, ��30���ַ���
    T6963_WR(0x00);
    T6963_CMD(_CMD_CTRL | _CMD_CTRL_GRAPHIC_AREA);
    
    //�����״
    T6963_CMD(_CMD_CURSOR | _CMD_CURSOR_8);
    
    //��ʾ��ʽ
    T6963_CMD(_CMD_MODE | _CMD_MODE_OR | _CMD_MODE_CG_ROM);
    
    LCD_ClearRAM();
    
    //��ʾ����
    T6963_CMD(_CMD_DISPLAY | _CMD_DISPLAY_GRAPHIC_ON);
}


/*
void delay(BYTE t)
{
    WORD data i;
    WORD data j;
    for(i = 0; i < t; ++i)
    {
        for(j = 0; j < 114; ++j);
    }
}


void write_data(BYTE dat)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat;
}

void write_cmd1(BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}

void write_cmd2(BYTE dat, BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}


void write_cmd3(BYTE dat1, BYTE dat2, BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat1;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat2;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}


void clear_screen()
{
    WORD i;
    write_cmd3(0x00, 0x00, 0x24);   //��ַָ������
    write_cmd1(0xB0);   //�����Զ�д����ַ�Լ�һ
    for(i = 0; i < 1024; ++i)
    {
        write_data(0xFF);
    }
    write_cmd1(0xB2);   //�����Զ�д����
}

void init_12864()
{
    RST_EN;
    delay(10);
    RST_UN;

    write_cmd3(0x01, 0x00, 0x21);   //���ָ������
    write_cmd3(0x00, 0x00, 0x40);   //�ı����׵�ַ
    write_cmd1(0x80);   //��ʾ��ʽ���ã�������ʾ
    write_cmd1(0x94);   //��ʾ״̬���� 1 0 1 0 N3 N2 N1 N0 �ı���ʾ����겻��ʾ������˸
    write_cmd1(0x98);   //ͼ�η�ʽ��ʾ������ʾ��ĸ��ֻ���
    write_cmd1(0xa1);   //�����״���� 1 0 1 0 0 N2 N1 N0
}
*/