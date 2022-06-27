#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"

void PPT_SW_init(void);
void LED_Init(void);

void LED_OFF(uint16_t name);
void LED_ON(uint16_t name);

#define ON 1
#define OFF 0



#endif 





