#ifndef _USART_
#define _USART_

#include "stm32f10x.h"
#include "Types.h"

void USART_Configuration(void);
void DMA_Configuration(void);
void NVIC_Configuration(void);

extern u8 USART2_SEND_DATA[64];
extern u8 USART2_RECEIVE_DATA[64];

extern u8 PA_V_H; /* ���ŵ�ѹ�߰�λ�����ŵ�ѹ����CPU�ϴ����� */
extern u8 PA_V_L; /* ���ŵ�ѹ�Ͱ�λ */
#endif            /* __USART1_H */
