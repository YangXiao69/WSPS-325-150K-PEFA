#ifndef __LCD_H
#define	__LCD_H

#include "stm32f4xx.h"


extern u8 Reset_Flag;
extern u8 waring_clear_flag;
extern u8 RestFlag;
extern u8 Alarm;              /* �����ܿ��� ��1Ϊ������*/
extern u8 Alarm_Volt;   
extern u8 Alarm_Curr;      
extern u8 Alarm_Temp;            /* �¶ȱ��� */
extern u8 Alarm_Pout;           /* �ܹ��ʱ��� */    
extern u8 Alarm_Pfout;           /* ���书�ʱ��� */
extern u8 Alarm_ZB;           /* פ������ */


extern u16 AlarmFlag;


extern u16 Alarm_Pout_Value       ;  //���ʱ�������
extern u16 Alarm_Pfout_Value       ;  //���ʱ�������
 
extern u16 Alarm_RPD_MODE         ;  //���䱣��ģʽ
extern u16 Alarm_RPD_Value        ;  //���䱣������
extern u16 Alarm_Vswr_MODE        ;  //פ������ģʽ
extern u8 Alarm_Vswr_Value       ;  //פ����������
extern u16 Alarm_Flow_Value ;
extern u16 Alarm_FlowTemp_Value;
 
extern u16 Alarm_Temp2_Value       ;  //�¶ȱ�������
extern u8 Alarm_Curr2_Value       ;  //������������
extern u8 Alarm_Vswr2_Value ;   // ���                               
extern u16 Alarm_Pout2_Value;   // ���

extern u8 erroradr;

extern u8 Power ;              /* ��Դ״̬ �� 1  �� 0 */
extern u8 RF ;                 /* ��Ƶ     �� 1  ��0  */
extern u8 WorkMode ;           /* ����ģʽ ���� 1   ���� 0*/
extern u8 Duty ;               /* ռ�ձ� */
extern u8 Ttime ;              /* ���� */
extern u16 Pset ;              /* ��·˥�� */
extern u16 Phase ;             /* ��λ */
extern u16 PulseWidth ;        /* ���� */

extern u8 Power0;                 /* ��Դ״̬ �� 1  �� 0 */
extern u8 RF0;                    /* ��Ƶ     �� 1  ��0  */
extern u8 WorkMode0;              /* ����ģʽ ���� 1   ���� 0*/
	
extern u8 operate;                /* ���� */
extern u8 Adr;
extern u32 bound;

extern u8 mtset;
extern u8 Source;
extern u8 PWMS;
extern u8 Source0;            /* Դ */
extern u8 PWMS0;              /* pwmԴ */

extern u16 Freq;

extern u16 RegisterAddr;



extern u8 time[5];                /* ��ȡ��ʱ���ݴ� */
extern u8 time0[5];               /* ���ص�BCDʱ��ֵ */
extern u16 user_settings[2];


void lcd_receive(void);



void LCD_DisPA(uint8_t IPnum,uint8_t * Rx_b);
void LCD_DisErr(uint8_t adr_H, uint8_t adr_L);



void lcd_send_one(u16 data,u16 adrss) ;
void display_power(void);
void Wait_USART1_Send(void);
void lcd_send_pageone(void) ;
void lcd_send_pagetwo(void) ;
void lcd_send_pagemain(void);
void clear_lcd_display(u8 num);

void Send_for_waring_clear(void) ;



#endif 
