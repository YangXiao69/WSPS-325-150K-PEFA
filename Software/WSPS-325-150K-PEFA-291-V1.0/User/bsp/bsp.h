#ifndef __BSP_H
#define __BSP_H

#include "stm32l4xx.h"


#include "./bsp_adc/bsp_adc.h"
#include "./bsp_eeprom/bsp_at24c02.h"
#include "./bsp_ds18b20/bsp_ds18b20.h"
#include "./bsp_gpio/bsp_gpio.h"
#include "./bsp_spi/bsp_spi.h"



void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)









void BSP_Inits(void);

void bsp_delayus(uint32_t us);


void SystemClock_Config(void);


#endif

