/*
ʵ����Һ��ģ��ĵײ���������
�����ĳ�ʼ��������ָ��λ�õ����أ�
��ʾ�ַ����ַ�����
�����ĵײ�������ã��磺ƫѹ���á�VOP���á�֡Ƶ���õȣ�

ʵ���˶�ȡģ���¶ȣ����������ǰ�¶�ֵ��

���ߣ� ������
���ڣ�2013-08-24
�汾��V1.0



�޸���ʷ��
1. 2013-08-24 �������飬���ν���

2. 2013-08-25�������飬ʵ���˳�ʼ�����ײ��д��ƫѹ���ã�VOP���ã�֡Ƶ���ã�

3. 2013-08-26�������飬ʵ����8*8�ַ���ʾ���ַ�����ʾ��0--65535������ʾ������

4. 2013-08-27�������飬ʵ�����¶ȶ�ȡ���¶���ʾ���ܣ������ϴ����������

5. 2013-8-28�������飬ʵ������������ͼ������һ���ص���ʾ���ƣ�

6. 2013-08-29�������飬��һ�汾����������ʵ�����������ء��������֡�����ͼ��Ŀɿ��ƣ�


*/


#ifndef		__LCD_H__
#define		__LCD_H__

void dly(void){
	unsigned char n = 0x00;
	for(n=0;n<2;n++);
	
}

//SPI ����һ���ֽ�
void SPI_Send_Byte(unsigned char dat){
	unsigned char n = 0x00;
	
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		//dly();
		if((dat & 0x80) == 0x80){
			LCD_SDAIN = 1;
		}else{
			LCD_SDAIN = 0;
		}
		//dly();
		LCD_CLK = 1;
		//dly();
		dat = dat << 1;
	}
	
}

//��LCDд����������� ����д��ַ����д���Ȼ��д����
void SPI_Write(unsigned char sub_addr,unsigned char cmd_dat,unsigned char para){
	unsigned char n = 0x00;
	unsigned char m = 0x00;
	unsigned char tmp = 0x00;
	
	LCD_CLK = 0;
	LCD_CS = 0;
	//dly();
	tmp = sub_addr;
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		//dly();
		if((tmp & 0x80) == 0x80){
			LCD_SDAIN = 1;
		}else{
			LCD_SDAIN = 0;
		}
		//dly();
		LCD_CLK = 1;
		//dly();
		tmp = tmp << 1;
	}
	
	tmp = cmd_dat;
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		//dly();
		if((tmp & 0x80) == 0x80){
			LCD_SDAIN = 1;
		}else{
			LCD_SDAIN = 0;
		}
		//dly();
		LCD_CLK = 1;
		//dly();
		tmp = tmp << 1;
	}
	
	tmp = para;
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		//dly();
		if((tmp & 0x80) == 0x80){
			LCD_SDAIN = 1;
		}else{
			LCD_SDAIN = 0;
		}
		//dly();
		LCD_CLK = 1;
		//dly();
		tmp = tmp << 1;
	}

	LCD_CS = 1;

}

//XX������ѹֵ�����ô�/�رձ�ѹ���ܣ����ñ�ѹֵ
void Set_Charge_Pump(unsigned char enbl,unsigned char XX){
	
	if(enbl == DISABLE){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xc0));
	}else{
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xc8 | ((XX & 0x07) - 2)));
	}
	
}

//����Һ����������ѹ
void Set_VLCD(unsigned int value){
	unsigned int tmp = 0x0000;
	unsigned char dat = 0x00;
	
	value = value & 0x01ff;//�ܹ�9λ����ֵ
	
	tmp = value;
	tmp = tmp & 0xfff0;
	tmp = tmp >> 4;
	dat = (unsigned char)(tmp);
	
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x40 | dat));
	
	tmp = value;
	tmp = tmp & 0x000f;
	dat = (unsigned char)(tmp);
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x60 | dat));
	
}

void Set_Inversion_Mode(unsigned char mode){
	
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xb0 | (mode & 0x07)));
	
}
//���ô�/�ر���ʾ
void Set_Display_Mode(unsigned char mode){
	
	if(mode == DISPLAY_ON){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0x39);
	}else{
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0x38);
	}
}

//frm����֡Ƶֵ��Hz������֡Ƶֵ���Զ�ѡ��������ӽ��Ŀ����ò���ֵ
void Set_Frame_Rate(unsigned int frm){
	unsigned char n = 0x00;
	
	if(frm < FRAME_RATE_CODE[0]){//���С�����֡Ƶֵ
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xe7);//Ĭ��ֵ��80Hz
	}else{
		if(frm > FRAME_RATE_CODE[31]){//���֡Ƶֵ�������ֵ
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xe7);//�ָ�Ĭ��ֵ
		}else{//������ֵ��Χ��
			for(n=0;n<31;n++){
				if((FRAME_RATE_CODE[n] <= frm) && (frm < FRAME_RATE_CODE[n+1])){
					SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xe0 | n));
				}
			}
		}
	}
	
}

//bs-- bias������ƫѹֵ
void Set_Bias(unsigned char bs){
	
	switch(bs){
		case 4:// 1/4bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd0);
		break;
		case 3:// 1/3bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd3);
		break;
		case 2:// 1/2bias
			SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,0xd2);
		break;
		default:
			
		break;
		
	}
}

//������һ��������ֵ
//Y = 0 ��ӦCOM0����COM7��
//Y = 1 ��ӦCOM8
//X �����÷�Χ 0����101
void Set_Position(unsigned char Y,unsigned char X){
	unsigned char tmp = 0x00;
	
	tmp = X;
	tmp = tmp >> 4;
	tmp = tmp & 0x07;
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x80 | tmp));//дX�ĸ�3λ
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0x90 | (X & 0x0f)));//дX�ĵ���λ
	
	SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,(0xa0 | (Y & 0x01)));//дY��ַ
	
	
}

//������һ���غ���������ָ��Y��ַ��X��ַдһ���ֽڵ�����
//��Y=1ʱ��ֻ��DB0��Ч��Y=0ʱ��DB7--DB0����Ч��
void Draw_Line(unsigned char Y,unsigned char X, unsigned char dat1,unsigned char dat2,unsigned char CNT){
	unsigned char n = 0x00;
	
	Set_Position(Y,X);
	
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	
	for(n=0;n<CNT;n++){
		SPI_Send_Byte(dat1);
		//Delayms();
		n++;
		//if(n<CNT){
		SPI_Send_Byte(dat2);
			//Delayms();
		//}
		
	}
	if((CNT & 0x01) == 0x00){
		SPI_Send_Byte(0x00);
	}
	
	
	LCD_CS = 1;
	
}


//���LCD�����е����ݣ�ʹ֮��ʾ�հ�
void Clear_LCD_ALL(void){
	unsigned char y = 0x00;

	for(y=0;y<Y_SIZE;y++){
		Draw_Line(y,0x00,0x00,0x00,SEG_CNT);
	}
	
	for(y=0;y<SEG_CNT;y++){
		LCD_RAM[y] = 0x00;
		
	}
	
	Current_X_POS = 0x00;
	
}


void Clear_LCD_Dianzhen(void){//���������
	unsigned char n = 0x00;
		
	Draw_Line(0x00,0x00,0x00,0x00,X_SIZE);
	
	Current_X_POS = 0x00;
	for(n=0;n<X_SIZE;n++){
		LCD_RAM[n] = 0x00;
		
	}
	
}

void Clear_LCD_NUM_ICON(void){//������֡�ͼ����
	
	Draw_Line(0x00,NUM_START_ADDR,0x00,0x00,(SEG_CNT - NUM_START_ADDR));
	Draw_Line(0x01,0x00,0x00,0x00,SEG_CNT);
	
}

//ˢ�µ���������
void LCD_Refresh_Dianzhen(void){
	unsigned char n = 0x00;
	
	Set_Position(0x00,0x00);//����Y=0��X=0
	
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	
	for(n=0;n<X_SIZE;n++){
		SPI_Send_Byte(LCD_RAM[X_SIZE - 1 - n]);
		//Delayms();
		
	}
	
	LCD_CS = 1;
	
}

//ˢ������������
void LCD_Refresh_NUM(void){
	unsigned char n = 0x00;
	
	Set_Position(0x00,NUM_START_ADDR);//����Y=0��X=95
	
	LCD_CS = 0;
	SPI_Send_Byte(WRITE_MODE);
	SPI_Send_Byte(WRITE_RAM_BYTE);
	
	
	for(n=NUM_START_ADDR;n<SEG_CNT;n++){
		SPI_Send_Byte(LCD_RAM[n]);
		//Delayms();
		
	}
	SPI_Send_Byte(0x00);
	
	LCD_CS = 1;
	
}

//��ָ��λ����ʾһ��8*8���ַ�
void Show_ASCII88(unsigned char X,unsigned char dat,unsigned char en_dis){//X = 0--12
	unsigned char n = 0x00;
	unsigned int tmp = 0x0000;
	unsigned char chr = 0x00;
	
	
	dat = dat - 32;
	tmp = dat;
	
	for(n=0;n<7;n++){
		chr = ASCII_ZX_DX[tmp*8+ n];
		LCD_RAM[X*7 + n] = chr;
		Draw_Line(0x00,94-X*7-n,chr,chr,0x01);
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_Dianzhen();
	}
	
}

//��ָ��λ����ʾһ���ַ���
void Show_String_88(unsigned char X,unsigned char* DAT){
	unsigned char n = 0x00;
	
	n = 0x00;
	while(DAT[n] != 0x00){
		if(DAT[n+1] != 0x00){
			Show_ASCII88((X+n),DAT[n],DISABLE);
		}else{
			Show_ASCII88((X+n),DAT[n],ENABLE);
		}
		
		n++;
	}
	
}


void Show_Current_Pos_Char(unsigned char dat){
	
	
	Show_ASCII88(Current_X_POS,dat,DISABLE);
	
	Current_X_POS++;
	
	if(Current_X_POS == 13){
		Current_X_POS = 0x00;
	}
	
}


//��ʼ���¶�ģ���Ƿ�򿪣������Ƿ���ˣ�VOP�Ƿ񲹳�
void LCD_Temperature_Init(unsigned char Measure_en,unsigned char Measure_filter,unsigned char Vop_Buchang){
	unsigned char cmd = 0x00;
	
	if(Measure_en){//����ʹ��
		cmd = cmd | 0x01;
	}
	if(Measure_filter){//�¶��˲�ʹ��
		cmd = cmd | 0x02;
	}
	if(Vop_Buchang){//VOP����ʹ��
		cmd = cmd | 0x04;
		
	}
	
	SPI_Write(WRITE_MODE,Temp_CTRL,cmd);
	
	
}

void LCD_Init(void){//Һ������ʼ��
	unsigned char n = 0x00;
	
	LCD_RST = 0;
	Delays(5);
	LCD_RST = 1;
	Delays(5);
	
	
	while(LCD_Init_COMMAND[n] != 0xff){
		SPI_Write(WRITE_MODE,LAST_CTRL_BYTE,LCD_Init_COMMAND[n]);
		n++;
		Delays(1);
	}
	
	Set_Charge_Pump(ENABLE,3);//3��ѹ
	Set_VLCD(75);//����VOP = 6.V// VOP��ѹ��Χ��3.99V--12V
	Delays(1);
	Set_Inversion_Mode(3);//3-line inversion
	Delays(1);
	Set_Frame_Rate(80);//����֡Ƶ 80Hz
	Delays(1);
	
	LCD_Temperature_Init(ENABLE,DISABLE,DISABLE);//ֻ���¶Ȳ���
	
	
	Clear_LCD_ALL();
	
	Set_Display_Mode(DISPLAY_ON);
	Delays(1);
	
	

}


#define	ZHENG		0
#define	ONE_NUM		1
#define	FU			2


void Show_8NUM(unsigned char num,unsigned char en_dis){
	switch(num){
		case ZHENG:
				LCD_RAM[100] = LCD_RAM[100] & 0x7f;//8G����ʾ����ʾ��ֵ
				LCD_RAM[99]  = LCD_RAM[99] & 0x7f;//8����ʾ
		break;
		case FU:
				LCD_RAM[100] = (LCD_RAM[100] & 0x7f) | 0x80;//8G��ʾ����ʾ��ֵ
				
		break;
		case ONE_NUM:
				LCD_RAM[99]  = (LCD_RAM[99] & 0x7f) | 0x80;//8��ʾ1
				
		break;
		default:
				LCD_RAM[99]  = LCD_RAM[99] & 0x7f;//8����ʾ
				LCD_RAM[100] = LCD_RAM[100] & 0x7f;//8����ʾ
		break;
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
	
}

const unsigned char NUM_9_10_CODE[]={
	0x50,0xf0,//0
	0x50,0x00,//1
	0x60,0xb0,//2
	0x70,0x90,//3
	0x70,0x40,//4
	0x30,0xd0,//5
	0x30,0xf0,//6
	0x50,0x80,//7
	0x70,0xf0,//8
	0x70,0xd0,//9
	0x70,0xe0,//A
	0x30,0x70,//b
	0x00,0xf0,//C
	0x70,0x30,//d
	0x20,0xf0,//E
	0x20,0xe0,//F
	
};

void Show_9NUM(unsigned char dat,unsigned char en_dis){
	if(dat < 16){
		LCD_RAM[99]  = (LCD_RAM[99] & 0x8f) | NUM_9_10_CODE[dat*2];
		LCD_RAM[101] = (LCD_RAM[101] & 0x0f) | NUM_9_10_CODE[dat*2+1];
		
	}else{
		LCD_RAM[99]  = (LCD_RAM[99] & 0x8f);
		LCD_RAM[101] = (LCD_RAM[101] & 0x0f);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

void Show_10NUM(unsigned char dat,unsigned char en_dis){
	if(dat < 16){
		LCD_RAM[97] = (LCD_RAM[97] & 0x8f) | NUM_9_10_CODE[dat*2];
		LCD_RAM[98] = (LCD_RAM[98] & 0x0f) | NUM_9_10_CODE[dat*2+1];
		
	}else{
		LCD_RAM[97] = (LCD_RAM[97] & 0x8f);
		LCD_RAM[98] = (LCD_RAM[98] & 0x0f);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

#define	ICON_SHESHIDU		130
void Show_11NUM(unsigned char chn,unsigned char en_dis){
	if(chn == ICON_SHESHIDU){//��ʾ���϶ȡ�
		LCD_RAM[96] = (LCD_RAM[96] & 0x3f) | 0x40;
		LCD_RAM[97] = (LCD_RAM[97] | 0x80);
		
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0x3f);
		LCD_RAM[97] = (LCD_RAM[97] & 0x7f);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}


unsigned char Read_Temperatue(void){// T = 0.6275 * [T] - 40
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	LCD_CLK = 0;
	LCD_CS = 0;
	
	SPI_Send_Byte(READ_MODE);//
	SPI_Send_Byte(0x80);//��ȡ�¶ȵ�����

	tmp = 0x00;
	for(n=0;n<8;n++){
		LCD_CLK = 0;
		tmp = tmp << 1;
		if(LCD_SDAOUT){
			tmp = tmp + 0x01;
		}
		LCD_CLK = 1;
		
	}

	LCD_CS = 1;
	
	return tmp;
	
}

void Show_12NUM(unsigned char dat,unsigned char en_dis){
	switch(dat){
		case 0x00://����ʾ
				LCD_RAM[95] = LCD_RAM[95] & 0x1f;//����ʾ
		break;
		case 0x01://��ʾ1
				LCD_RAM[95] = (LCD_RAM[95] & 0x1f) | 0xa0;//��ʾBC-- 1
		break;
		case 0x02://��ʾ2
				LCD_RAM[95] = (LCD_RAM[95] & 0x1f) | 0xc0;//��ʾ2
		break;
		default:
				LCD_RAM[95] = LCD_RAM[95] & 0x1f;//����ʾ
		break;
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

const unsigned char NUM_13_14_CODE[]={//
	0x0f,0x0a,//0
	0x00,0x0a,//1
	0x0d,0x06,//2
	0x09,0x0e,//3
	0x02,0x0e,//4
	0x0b,0x0c,//5
	0x0f,0x0c,//6
	0x01,0x0a,//7
	0x0f,0x0e,//8
	0x0b,0x0e,//9
	0x07,0x0e,//A
	0x0e,0x0c,//b
	0x0f,0x00,//C
	0x0c,0x0e,//d
	0x0f,0x04,//E
	0x07,0x04,//F
	
};

void Show_13NUM(unsigned char dat,unsigned char en_dis){
	if(dat < 16){
		LCD_RAM[95] = (LCD_RAM[95] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[96] = (LCD_RAM[96] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//����ʾ
		LCD_RAM[95] = (LCD_RAM[95] & 0xf0);
		LCD_RAM[96] = (LCD_RAM[96] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

void Show_14NUM(unsigned char dat,unsigned char en_dis){
	if(dat < 16){
		LCD_RAM[97] = (LCD_RAM[97] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[98] = (LCD_RAM[98] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//����ʾ
		LCD_RAM[97] = (LCD_RAM[97] & 0xf0);
		LCD_RAM[98] = (LCD_RAM[98] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

void Show_15NUM(unsigned char dat,unsigned char en_dis){
	if(dat < 16){
		LCD_RAM[99]  = (LCD_RAM[99] & 0xf0) | NUM_13_14_CODE[dat*2];
		LCD_RAM[101] = (LCD_RAM[101] & 0xf1) | NUM_13_14_CODE[dat*2+1];
		
	}else{//����ʾ
		LCD_RAM[99] = (LCD_RAM[99] & 0xf0);
		LCD_RAM[101] = (LCD_RAM[101] & 0xf1);
		
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}



#define		ICON_SNOW		129
void Show_Snow(unsigned char chn,unsigned char en_dis){
	if(chn == ICON_SNOW){
		LCD_RAM[96] = (LCD_RAM[96] | 0x20);
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0xdf);
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

#define		ICON_MAOHAO		128
void Show_Maohao(unsigned char chn,unsigned char en_dis){
	if(chn == ICON_MAOHAO){
		LCD_RAM[96] = (LCD_RAM[96] | 0x01);
	}else{
		LCD_RAM[96] = (LCD_RAM[96] & 0xfe);
	}
	
	if(en_dis == ENABLE){
		LCD_Refresh_NUM();
	}
	
}

void Show_Time(unsigned char hour,unsigned char minute,unsigned char second,unsigned char en_dis){
	
	if(en_dis){
		Show_12NUM((hour/10),DISABLE);
		Show_13NUM((hour%10),DISABLE);
		Show_14NUM((minute/10),DISABLE);
		Show_15NUM((minute%10),DISABLE);
		
		Show_Maohao((ICON_MAOHAO + (second & 0x01)),ENABLE);
		
	}else{
		Show_12NUM((0xff),DISABLE);
		Show_13NUM((0xff),DISABLE);
		Show_14NUM((0xff),DISABLE);
		Show_15NUM((0xff),DISABLE);
		
		Show_Maohao((ICON_MAOHAO + 0x01),ENABLE);
		
	}
	
	
}


#define		ICON_AUTO			0
#define		ICON_WEST			1
#define		ICON_SOUTH			2
#define		ICON_CITY			3
#define		ICON_EAST			13
#define		ICON_MILE			17
#define		ICON_KM_M			18
#define		ICON_KM_K			23
#define		ICON_FRONT_LIGHT		87
#define		ICON_CONFIG			70
const unsigned ICON_CODE[]={
	0,
	1,
	2,
	3,
	13,
	17,
	18,
	23,
	87,
	70,
	
};
void Show_Icon(unsigned char chn,unsigned char en_dis){
	unsigned char n = 0x00;
	
	for(n=0;n<10;n++){
		if(chn == ICON_CODE[n]){//��ָ����ͼ��
			if(en_dis == ENABLE){
				Draw_Line(0x01,chn,0x01,0x01,0x01);//��ʾ��Ӧͼ��
			}else{
				Draw_Line(0x01,chn,0x00,0x00,0x01);//����ʾ��Ӧͼ��
			}
			
			goto SHOW_ICON_EXIT;
		}
	}
	
	if(chn == ICON_SHESHIDU){//����ǿ��ơ�ͼ��
		Show_11NUM((ICON_SHESHIDU+(1-en_dis)),ENABLE);
	}else{
		if(chn == ICON_SNOW){//����ǿ���ѩ��ͼ��
			Show_Snow((ICON_SNOW+(1-en_dis)),ENABLE);
		}else{
			if(chn == ICON_MAOHAO){//����ǿ���ʱ��ð��ͼ��
				Show_Maohao((ICON_MAOHAO+(1-en_dis)),ENABLE);
			}
		}
	}
	
	
SHOW_ICON_EXIT:
	n = 0x00;
	
}

void Show_Temperatue(void){
	unsigned char tmp = 0x00;
	unsigned long dat = 0x00000000;
	unsigned int TT = 0x0000;
	unsigned char bai = 0x00;
	unsigned char shi = 0x00;
	unsigned char ge = 0x00;
	
	tmp = Read_Temperatue();
	
	if(tmp != 0xff){//�����������ݲ�����0xff
		if(tmp > 63){//���¶�ֵ
			dat = tmp;
			dat = dat * 6275;
			dat = dat - (400000);//�¶�ֵ�Ŵ�10000��
			dat = dat + 5000;// �¶�ֵ��������
			dat = dat / 10000;// �¶�ֵ�ָ�����
			
			TT = (unsigned int)(dat);//ȡ������ʾ�¶���������ʾС��
			
			bai = TT / 100;//�¶Ȱ�λֵ
			shi = (TT - bai*100)/10;//�¶�ʮλֵ
			ge = TT % 10;//�¶ȸ�λֵ
			
			
			Show_8NUM(bai,DISABLE);
			Show_9NUM(shi,DISABLE);
			Show_10NUM(ge,DISABLE);
			
		}else{
			dat = tmp;
			dat = dat * 6275;
			dat = (400000) - dat; // ���¶�ֵ
			dat = dat - 5000;//�¶�ֵ ��������
			dat = dat / 10000;// �¶Ȼظ�����
			
			TT = (unsigned int)(dat);
			
			shi = TT / 10;//�¶�ʮλֵ
			ge = TT % 10;//�¶ȸ�λֵ
			
			Show_8NUM(FU,DISABLE);
			Show_9NUM(shi,DISABLE);
			Show_10NUM(ge,DISABLE);
			Show_Snow(ICON_SNOW,DISABLE);
			
		}
		
		Show_11NUM(ICON_SHESHIDU,ENABLE);
		
		
	}
	
	
}


const unsigned char NUM_1_CODE[]={//D, C, B, A, F, G, E
	0xfa,//0
	0x60,//1
	0xb6,//2
	0xf4,//3
	0x6c,//4
	0xdc,//5
	0xde,//6
	0x70,//7
	0xfe,//8
	0xfc,//9
	0x7e,//A
	0xce,//b
	0x9a,//C
	0xe6,//d
	0x9e,//E
	0x1e,//F
	
};

void Show_1NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_1_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(88+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(88+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(88+n),0x00,0x00,0x01);
		}
	}
	
}

const unsigned char NUM_2_3_CODE[]={// C, B, A, F, G, E, D 
	0xf6,//0
	0xc0,//1
	0x6e,//2
	0xea,//3
	0xd8,//4
	0xba,//5
	0xbe,//6
	0xe0,//7
	0xfe,//8
	0xfa,//9
	0xfc,//A
	0x9e,//b
	0x36,//C
	0xce,//d
	0x3e,//E
	0x3c,//F
	
};

void Show_2NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(59+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(59+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(59+n),0x00,0x00,0x01);
		}
	}
	
}

void Show_3NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(52+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(52+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(52+n),0x00,0x00,0x01);
		}
	}
	
}

void Show_4NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(45+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(45+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(45+n),0x00,0x00,0x01);
		}
	}
	
}

void Show_5NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(38+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(38+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(38+n),0x00,0x00,0x01);
		}
	}
	
}

void Show_6NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(31+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(31+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(31+n),0x00,0x00,0x01);
		}
	}
	
}

void Show_7NUM(unsigned char dat){
	unsigned char n = 0x00;
	unsigned char tmp = 0x00;
	
	if(dat < 16){
		tmp = NUM_2_3_CODE[dat];
		
		for(n=0;n<7;n++){
			if((tmp & 0x80) == 0x80){
				Draw_Line(0x01,(24+n),0x01,0x01,0x01);
			}else{
				Draw_Line(0x01,(24+n),0x00,0x00,0x01);
			}
			tmp = tmp << 1;
		}
	}else{
		for(n=0;n<7;n++){
			Draw_Line(0x01,(24+n),0x00,0x00,0x01);
		}
	}
	
}

const unsigned int MI_CODE[]={
	0x0955,//0
	0x0005,//1
	0x0996,//2
	0x0897,//3
	0x00c7,//4
	0x08d3,//5
	0x09d3,//6
	0x0015,//7
	0x09d7,//8
	0x08d7,//9
	0x01d7,//A
	0x09c3,//b
	0x0950,//C
	0x0987,//d
	0x09d2,//E
	0x01d2,//F
	
};

void Show_Mi(unsigned char dat){
	unsigned char n = 0x00;
	unsigned int tmp = 0x00;
	
	if(dat < 16){
		tmp = MI_CODE[dat];
		
		for(n=0;n<13;n++){
			if(n != 9){//��һ����ַ���˵�ַΪ��E��
				if((tmp & 0x0001) == 0x0001){
					Draw_Line(0x01,(4+n),0x01,0x01,0x01);
				}else{
					Draw_Line(0x01,(4+n),0x00,0x00,0x01);
				}
			}
			
			
			tmp = tmp >> 1;
		}
	}else{
		for(n=0;n<13;n++){
			if(n != 9){//��һ����ַ���˵�ַΪ��E��
				Draw_Line(0x01,(4+n),0x00,0x00,0x01);
			}
			
		}
	}
	
}

//����������ͳһ����
void Show_Multi_NUM(unsigned char chn,unsigned char num){
	
	switch(chn){
		case 0x01://��1��7������
					Show_1NUM(num);
		break;
		case 0x02://��2��7������
					Show_2NUM(num);
		break;
		case 0x03://��3��7������
					Show_3NUM(num);
		break;
		case 0x04://��4��7������
					Show_4NUM(num);
		break;
		case 0x05://��5��7������
					Show_5NUM(num);
		break;
		case 0x06://��6��7������
					Show_6NUM(num);
		break;
		case 0x07://��7��7������
					Show_7NUM(num);
		break;
		case 0x08://��8��7������
					Show_8NUM(num,ENABLE);
		break;
		case 0x09://��9��7������
					Show_9NUM(num,ENABLE);
		break;
		case 10://��10��7������
					Show_10NUM(num,ENABLE);
		break;
		case 11://��11��7������
					Show_11NUM(num,ENABLE);
		break;
		case 12://��12��7������
					Show_12NUM(num,ENABLE);
		break;
		case 13://��13��7������
					Show_13NUM(num,ENABLE);
		break;
		case 14://��14��7������
					Show_14NUM(num,ENABLE);
		break;
		case 15://��15��7������
					Show_15NUM(num,ENABLE);
		break;
		case 16://��ʾ�� �ַ�
				Show_Mi(num);
				
		break;
		
		default:
			
		break;
		
	}
	
}

void Test_LCD(void){
	
	Show_String_88(0,(unsigned char *)"   Dushanwu  ");
	Delays(DELAY_VALUE);
	
	
}


#endif

