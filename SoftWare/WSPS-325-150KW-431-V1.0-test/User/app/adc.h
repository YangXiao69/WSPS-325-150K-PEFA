#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "Types.h"

void ADC1_Init(void);
void adc_date_dispose(void);
void CURR_calculate(void);
void POWER_calculate(void);

extern vu16 Current[4]; /* ��β�����ȡƽ��ֵ */

extern u16 VOLT_DEC;
extern float Pout;
extern float Pfout;
extern float Pin;
extern u8 ADC_CUR_OK;
#endif /* __ADC_H */
