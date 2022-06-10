#ifndef _USART_
#define _USART_

#include "stm32f10x.h"
#include "Types.h"

void USART_Configuration(void);
void DMA_Configuration(void);
void NVIC_Configuration(void);

extern u8 USART2_SEND_DATA[64];
extern u8 USART2_RECEIVE_DATA[64];

extern u8 PA_V_H; /* 功放电压高八位，功放电压由子CPU上传过来 */
extern u8 PA_V_L; /* 功放电压低八位 */
#endif            /* __USART1_H */
