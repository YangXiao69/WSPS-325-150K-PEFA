#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
//#include "stm32f10x_can.h"

void CAN_Config(void);
void CAN_SetMsg(void);
void can_reseve(void);

extern CanTxMsg TxMessage; //���ͻ�����
extern CanRxMsg RxMessage; //���ջ�����
extern u8 Can_Rseve_Flag;  //�յ����ݱ�־
#endif
