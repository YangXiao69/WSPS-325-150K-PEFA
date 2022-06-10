#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


//引脚定义
/*******************************************************/

//#define USARTx_CLK                         RCC_APB1Periph_USART2
//#define USARTx_CLOCKCMD                    RCC_APB1PeriphClockCmd
//#define USARTx_BAUDRATE                    115200  //串口波特率

//#define USARTx_RX_GPIO_PORT                GPIOD
//#define USARTx_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
//#define USARTx_RX_PIN                      GPIO_Pin_6
//#define USARTx_RX_AF                       GPIO_AF_USART2
//#define USARTx_RX_SOURCE                   GPIO_PinSource6

//#define USARTx_TX_GPIO_PORT                GPIOD
//#define USARTx_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
//#define USARTx_TX_PIN                      GPIO_Pin_5
//#define USARTx_TX_AF                       GPIO_AF_USART2
//#define USARTx_TX_SOURCE                   GPIO_PinSource5

/************************************************************/

void USART2_Config(void);

void USART1_Config(u32 bound);
void USART3_Config(u32 bound);

void UPUSART1_Config(u32 bound);

void UPUSART3_Config(u32 bound);


void USART2_Send(u8 len);
void USART1_Send(u8 len);
void USART3_Send(u8 len);
void Wait_USART2_Send(void);
void Wait_USART3_Send(void);

extern u8 UART2_DMA_RX[64];
extern u8 UART2_DMA_TX[64];
extern u8 UART2_TX_Finish;

extern u8 UART1_DMA_RX[256];
extern u8 UART1_DMA_TX[256];
extern u8 UART1_TX_Finish;

extern u8 UART3_DMA_RX[256];
extern u8 UART3_DMA_TX[256];
extern u8 UART3_TX_Finish;
extern u32 bounddata[5];
extern u8 BoundNum232;
extern u8 BoundNum485;



#endif /* __USART_H */
