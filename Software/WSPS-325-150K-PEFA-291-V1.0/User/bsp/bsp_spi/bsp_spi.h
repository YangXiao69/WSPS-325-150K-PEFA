#ifndef __BSP_SPI_H
#define __BSP_SPI_H


#include "stm32l4xx_hal.h"



void SPI2_Init(void);
uint8_t SPI2_SendByte(uint8_t byte);


#endif
