#ifndef   CONFIG_H 
#define   CONFIG_H 

/***********************************************************************
*MCU stc12c5a60s2
*���� 32MHz
*LCD1602 4 λ�����߳���
*   �˿�����              �˿ڷ���    LCD���š�  MCU�˿�   
*   RS����/ָ��ѡ����        RS         4         P0^6
*   R/W��дѡ����            R/W        5         P0^5
*   ENдʹ����               EN         6         P0^4
*   ��������                 D0-D3     7-10       NC
*                            D4         11        P0^3
*                            D5         12        P0^2
*                            D6         13        P0^1
*                            D7         14        P0^0
*   �����˿����ӣ�
*   ��Դ��                   VSS        1      
*   ��Դ��+4.5--+5.5V        VDD        2
*   �Աȶȵ��� (�ӵ����)    VL         3 
*   ���⸺                   BLK VSS    15    
*   ������                   BLA VDD    16
***********************************************************************/

//#define LCD_8BIT
//#define LCD_FUNCTION	0x38	// LCD mode as 8-bit 2-line 5*8-dot 1/16Duty
#define LCD_4BIT
#define LCD_FUNCTION	0x28	// LCD mode as 4-bit 2-line 5*8-dot 1/16Duty

/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
#define LCDPort P0

/* Pins E and RS of LCD must be assigned to LCDControlPort*/
#define LCDControlPort P0

/* LCD Enable pin is assigned to Px1 */
#define LCD_Enable_Pin    4
/* LCD R/W pin is assigned to Px5 */
#define LCD_RW_Pin        5
/* LCD RS pin is assigned to Px2 */
#define LCD_RS_Pin        6

#endif
