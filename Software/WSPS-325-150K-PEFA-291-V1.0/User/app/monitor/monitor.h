#ifndef __MONITOR_H
#define __MONITOR_H



#include "stm32l4xx.h"




extern uint8_t Power;            /* �����ܿ��� ��1Ϊ������*/
 
extern uint8_t Power_V;            /* ��ѹ���� */
extern uint8_t Power_I;            /* �������� */
extern uint8_t Power_T;            /* �¶ȱ��� */
extern uint8_t Power_VP;           /* �ܹ��ʱ��� */
extern uint8_t Power_VF;           /* ���书�ʱ��� */
extern uint8_t Power_ZB;           /* פ������ */

extern uint8_t ErrorFlag;         /* ��0�����쳣   0xff�����Ѿ�������쳣*/

extern uint8_t PowerFlag[2];  /* �쳣λ��ʾ���� */



extern uint8_t Alarm_Curr_Value;           // ���
extern uint8_t Alarm_Vswr_Value;           // ���
extern uint16_t Alarm_Pout_Value;          // ���
extern uint16_t Alarm_Temp_Value;          // ���



void Timing_Protect_Control(void);

void set_data(uint8_t *paTxbuff);

void error_serve(void);

void CURR_calculate(void);

#endif

