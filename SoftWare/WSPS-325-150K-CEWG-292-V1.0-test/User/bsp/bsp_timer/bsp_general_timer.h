#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

#define GENERAL_TIM           		TIM2
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn			TIM2_IRQn
#define GENERAL_TIM_IRQHandler      TIM2_IRQHandler



typedef struct _tag_runTime
{
    uint8_t ucYear;
    uint8_t ucMonth;
    uint8_t ucDate;
    uint8_t ucHour;
    uint8_t ucMinute;
}RunTime;


extern __IO uint16_t g_ucWrtTimeCnt;
extern RunTime g_tTotalRunTime;


void TIMx_Configuration(void);


#endif /* __GENERAL_TIM_H */

