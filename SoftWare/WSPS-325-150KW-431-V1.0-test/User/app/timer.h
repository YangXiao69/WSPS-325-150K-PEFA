#ifndef TIMER_
#define TIMER_

#include "stm32f10x.h"
#include "Types.h"

#define START_TIME4                                      \
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); \
    TIM_Cmd(TIM4, ENABLE)
#define STOP_TIME4          \
    TIM_Cmd(TIM4, DISABLE); \
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE)

void TIM2_Configuration(void);
void TIM4_Configuration(void);

void staet_timer(void);
#endif /* TIME_TEST_H */
