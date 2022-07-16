#ifndef __LCD_RS232_H
#define __LCD_RS232_H

#include "main.h"
void Task_DC_Read_Data(void);
void Modbus_RS232_reseive(void);

extern uint8_t UART2_DMA_TX[64];
extern uint8_t UART2_DMA_RX[64];

#endif






