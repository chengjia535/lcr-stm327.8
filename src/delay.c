
#include "intrins.h"

//��ʱX΢��(STC12C5A60S2@12M) ��ͬ�Ĺ�������,��Ҫ�����˺���
void DelayXus(unsigned char n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}

// mS��ʱ����������������� unsigned char t���޷���ֵ
// unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
// 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayXus(245);
	 DelayXus(245);
 }
}
