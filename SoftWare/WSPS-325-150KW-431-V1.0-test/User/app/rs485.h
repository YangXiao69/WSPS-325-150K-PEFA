#ifndef __RS485_H
#define __RS485_H

#include "stm32f10x.h"

extern u8 USART1_RECEIVE_DATA[64];

void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf, u8 len);

#endif
