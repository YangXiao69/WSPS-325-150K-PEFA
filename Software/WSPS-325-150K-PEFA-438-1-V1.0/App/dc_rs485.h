#ifndef __DC_RS485_H
#define __DC_RS485_H

#include "main.h"
void Task_DC_Read_Data(void);
void DC_Power_Reivce(void);

extern uint8_t UART3_DMA_TX[64];
extern uint8_t UART3_DMA_RX[64];
extern UART_HandleTypeDef huart3;

#endif






