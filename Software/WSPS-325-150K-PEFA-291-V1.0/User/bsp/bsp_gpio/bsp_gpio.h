#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H


#include "stm32l4xx.h"



#define ON 1
#define OFF 0




extern uint8_t PPT;


void BoardAddr_Init(void);

void PPT_SW_Init(void);

void PPT_SW(uint8_t sw);



#endif
