#ifndef   CONFIG_H 
#define   CONFIG_H 

#define CPU_FOSC        32000000  //ϵͳʱ��32MHz

#define  READPointer          //������������ָ��ָ���������ͣ����㸴�ƣ�idata��xdata��code����:ͨ��ָ�룩
#define  WRITEPointer         //����д������ָ��ָ���������ͣ����㸴�ƣ�idata��xdata��code����:ͨ��ָ�룩

#define VFULL           9600
#define GAD             (9600/30)

sfr WDT_CONTR = 0xC1;

//====EEPROWƫ��======================
sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;


/* �˿����á�*/
sfr P1M1=0x91; //P1�˿����üĴ���
sfr P1M0=0x92; //P1�˿����üĴ���
sfr P0M1=0x93; //P0�˿����üĴ���
sfr P0M0=0x94; //P0�˿����üĴ���
sfr P2M1=0x95; //P2�˿����üĴ���
sfr P2M0=0x96; //P2�˿����üĴ���
sfr P3M1=0xB1; //P3�˿����üĴ���
sfr P3M0=0xB2; //P3�˿����üĴ���


sbit spk = P2^3; //������
sbit Kb = P2^1;  //���̿���B
sbit Ka = P2^2;  //���̿���A
sbit K3 = P1^7;
sbit K4 = P1^6;
sbit K5 = P1^5;  //7.8kHz�˲�����
sbit K6 = P1^4;
sbit K8 = P2^0;  //100Hz�˲�����



//����ȡֵ����
#define FUN_R		0  	  //���蹦��
#define FUN_C		1	  //���ݹ���
#define FUN_L		2	  //��й���
#define FUN_S		3	  //�������ܼ�

//����ȡֵ����
#define R40		0         //
#define R1K		1         //
#define R10K		2         //
#define R100K		3         //
#define AUTO		4	  //�������ܼ�

//Ƶ��ȡֵ����
#define F100		0         //100HZ
#define F1K		1         //1K
#define F7K8		2         //7.8K
                
//ģʽȡֵ����
#define MODE_S          0       //����ģʽ
#define MODE_P          1       //����ģʽ
#define MODE_M          2       //��������

//����ȡֵ����
#define GAIN1           0       //1��
#define GAIN3           1       //3��
#define GAIN9           2       //9��
#define GAIN27          3       //27��
#define GAIN_UP         4       //�ϱ�����
#define GAIN_DOWN       5       //�±�����

//����ֲ����
#define VX_A            0       //�ϱ�X����
#define VY_B            1       //�ϱ�Y����
#define VX_C            2       //�±�X����
#define VY_D            3       //�±�Y����

#endif
