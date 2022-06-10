#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"
#include "Types.h"

u8 DS18B20_Init(void); //��ʼ��DS18B20

void DS18B20_Get_Temp(void); //��ȡ�¶�

extern int16_t temp[9];

#define HIGH 1
#define LOW 0

#define DS18B20_CLK RCC_APB2Periph_GPIOC
#define DS18B20_PIN GPIO_Pin_12
#define DS18B20_PORT GPIOC

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DS18B20_DATA_OUT(a)                      \
	if (a)                                       \
		GPIO_SetBits(DS18B20_PORT, DS18B20_PIN); \
	else                                         \
		GPIO_ResetBits(DS18B20_PORT, DS18B20_PIN)
//��ȡ���ŵĵ�ƽ
#define DS18B20_DATA_IN() GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_PIN)

uint8_t DS18B20_Init2(void);
float DS18B20_Get_Temp2(void);

#endif
