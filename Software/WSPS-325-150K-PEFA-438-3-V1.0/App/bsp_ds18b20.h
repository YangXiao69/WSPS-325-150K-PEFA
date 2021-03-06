/**
  **************************************************************************************
  * @file    bsp.h
  * @author  
  * @version V1.00
  * @date    2020-11-20
  * @brief   bsp header
  * @attention
  *
  * 
  * 
  * 
  *
  **************************************************************************************
  */

#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

/*???Ŷ???*/
/************************************************************/

#define DS18B20_PIN GPIO_PIN_5
#define DS18B20_PORT GPIOB
#define DS18B20_CLK_ENABLE() __GPIOB_CLK_ENABLE()

/************************************************************/

void DS18B20_GPIO_Config(void);

#endif

/*********************************************END OF FILE*************************************/
