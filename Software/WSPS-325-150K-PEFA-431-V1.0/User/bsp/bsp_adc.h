#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

void ADC1_Init(void);
void adc_date_dispose(void);
void CURR_calculate(void);
void POWER_calculate(void);

extern uint16_t  ADC_Value_Power[9];

#endif /* __ADC_H */
