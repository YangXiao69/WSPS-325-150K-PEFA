#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

// ADC ��ź궨��
//#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0



void ADC_Config(void);
void adc_data_calculate(void);
void adc_data_calculate_static(void);

extern float PowerOut,PowerR,PowerZB;
extern float PfoutOut;
extern u16 V_data,V_data1,V_data2;
extern float Flow,FlowTemp;
extern float pfout_temp;
#endif /* __BSP_ADC_H */



