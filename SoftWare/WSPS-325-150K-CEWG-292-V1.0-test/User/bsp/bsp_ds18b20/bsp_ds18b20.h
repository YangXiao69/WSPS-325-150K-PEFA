#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H 

#include "stm32f4xx.h"

u8 DS18B20_Init(void);			//初始化DS18B20

void DS18B20_Get_Temp(void);	  //获取温度

//extern int16_t temp[9];   

#define HIGH  1
#define LOW   0

#define DS18B20_CLK     RCC_AHB1Periph_GPIOD
#define DS18B20_PIN     GPIO_Pin_3
#define DS18B20_PORT	GPIOD

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define   DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);\
					else		\
					GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN)
 //读取引脚的电平
#define  DS18B20_DATA_IN()	   GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN)

uint8_t DS18B20_Init2(void);
short DS18B20_Get_Temp2(void);

void Get_Flow_Temp(void);

#endif















