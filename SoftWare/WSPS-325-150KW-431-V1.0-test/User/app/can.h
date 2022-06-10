#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
//#include "stm32f10x_can.h"

void CAN_Config(void);
void CAN_SetMsg(void);
void can_reseve(void);

extern CanTxMsg TxMessage; //发送缓冲区
extern CanRxMsg RxMessage; //接收缓冲区
extern u8 Can_Rseve_Flag;  //收到数据标志
#endif
