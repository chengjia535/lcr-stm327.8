#ifndef _T6963_H
#define _T6963_H

//T6963C ״̬��־
// -----------------------------------------------------------------
// |  STA7 |  STA6 |  STA5 |  STA4 |  STA3 |  STA2 |  STA1 |  STA0 |
// -----------------------------------------------------------------
// STA7: ��˸״̬, 1Ϊ������ʾ, 0Ϊ�ر�
// STA6: �����־, ����ScreenPeek ��ScreenCopy����
// STA5: ����������������
// STA4: δʹ��
// STA3: ����Զ�ģʽ�µ�д����
// STA2: ����Զ�ģʽ�µĶ�����
// STA1: ������ݶ�д����
// STA0: �������ִ������
#define _STATE_CHK_RW                   (0x03)
#define _STATE_CHK_AUTO_RD              (0x04)
#define _STATE_CHK_AUTO_WR              (0x08)


//�Ĵ�����������(�ɸ�4λ��������͵�4λ�����������)
#define _CMD_REG                        (0x20)
#define _CMD_REG_CURSOR                 (0x01)
#define _CMD_REG_OFFSET                 (0x02)
#define _CMD_REG_ADDRESS                (0x04)

//��������������
#define _CMD_CTRL                       (0x40)
#define _CMD_CTRL_TEXT_HOME             (0x00)
#define _CMD_CTRL_TEXT_AREA             (0x01)
#define _CMD_CTRL_GRAPHIC_HOME          (0x02)
#define _CMD_CTRL_GRAPHIC_AREA          (0x03)

//ģʽ��������
#define _CMD_MODE                       (0x80)
#define _CMD_MODE_OR                    (0x00)
#define _CMD_MODE_EXOR                  (0x01)
#define _CMD_MODE_AND                   (0x03)
#define _CMD_MODE_TEXT                  (0x04)
#define _CMD_MODE_CG_ROM                (0x00)
#define _CMD_MODE_CG_RAM                (0x08)

//��ʾģʽ
#define _CMD_DISPLAY                    (0x90)
#define _CMD_DISPLAY_OFF                (0x00)
#define _CMD_DISPLAY_BLINK_ON           (0x01)
#define _CMD_DISPLAY_CURSOR_ON          (0x02)
#define _CMD_DISPLAY_TEXT_ON            (0x04)
#define _CMD_DISPLAY_GRAPHIC_ON         (0x08)

//�α���״
#define _CMD_CURSOR                     (0xA0)
#define _CMD_CURSOR_1                   (0x00)
#define _CMD_CURSOR_2                   (0x01)
#define _CMD_CURSOR_3                   (0x02)
#define _CMD_CURSOR_4                   (0x03)
#define _CMD_CURSOR_5                   (0x04)
#define _CMD_CURSOR_6                   (0x05)
#define _CMD_CURSOR_7                   (0x06)
#define _CMD_CURSOR_8                   (0x07)

//�����Զ���д
#define _CMD_AUTO                       (0xB0)
#define _CMD_AUTO_WRITE                 (0xB0)
#define _CMD_AUTO_READ                  (0xB1)
#define _CMD_AUTO_RESET                 (0xB2)

//���ݶ�д
#define _CMD_DATA                       (0xC0)
#define _CMD_DATA_WR_INCREASE           (0xC0)
#define _CMD_DATA_RD_INCREASE           (0xC1)
#define _CMD_DATA_WR_DECREASE           (0xC2)
#define _CMD_DATA_RD_DECREASE           (0xC3)
#define _CMD_DATA_WR_NO_MOVE            (0xC4)
#define _CMD_DATA_RD_NO_MOVE            (0xC5)

//��Ļ��
#define _CMD_SCREEN_PEEK                (0xE0)

//��Ļ����
#define _CMD_SCREEN_COPY                (0xE8)

//λ����
#define _CMD_BIT                        (0xF0)
#define _CMD_BIT_CLR(x)                 (0xF0 | (x))
#define _CMD_BIT_SET(x)                 (0xF8 | (x))

typedef enum
{
    Increase = 1,
    Decrease = -1,
    No_move  = 0,
} enum_Dir;

void T6963_CMD(BYTE x);
void T6963_WR(BYTE x);
BYTE T6963_RD(void);
void T6963_AutoWR(BYTE x);
BYTE T6963_AutoRD(void);
BYTE T6963_ReadData(enum_Dir dir);
void T6963_WriteData(BYTE dat, enum_Dir dir);
void T6963_InitBus(void);


#endif //_T6963_H