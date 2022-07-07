#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32h7xx.h"


#define DS18B20_HIGH GPIO_PIN_SET
#define DS18B20_LOW GPIO_PIN_RESET

#define Bit_RESET 0
#define Bit_SET 1



uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(uint8_t i);
void DS18B20_ReadId(uint8_t *ds18b20_id);


#endif

