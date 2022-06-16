#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"


#define DS18B20_HIGH 1
#define DS18B20_LOW 0


uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(uint8_t i);
void DS18B20_ReadId(uint8_t *ds18b20_id);


#endif

