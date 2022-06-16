#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H

#include "stm32f10x.h"


/*Òý½Å¶¨Òå*/
/************************************************************/

#define DS18B20_PIN GPIO_Pin_3
#define DS18B20_PORT GPIOD
#define DS18B20_CLK_ENABLE() __GPIOD_CLK_ENABLE()

/************************************************************/

void DS18B20_GPIO_Config(void);

#endif



/*********************************************END OF FILE*************************************/
