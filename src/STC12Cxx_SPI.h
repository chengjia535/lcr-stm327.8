#ifndef     STC12Cxx_SPI_H
#define     STC12Cxx_SPI_H


//ͨ�Ŷ˿�����
sbit SCK = P0^0;
sbit MISO = P0^1; //����������
sbit MOSI = P0^2; //���������

sbit cog_cs = P0^3;
sbit cog_rst = P0^4;

//void InitSPI();
uchar SPI_Sends(uchar out_data);

 

#endif













