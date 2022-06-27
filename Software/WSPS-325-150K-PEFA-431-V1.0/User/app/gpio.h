#ifndef __GPIO_H
#define __GPIO_H
#include "system.h"
#include "stm32f10x.h"

void Addr_Init(void);
void PPT_SW(uint8_t sw);
void LED_ON(uint16_t name);
void LED_OFF(uint16_t name);
void LED_COML(void);

#define POWER_LED 1
#define RFOUT_LED 2
#define VSWR_LED 3
#define CURR_LED 4
#define TEMP_LED 5
#define COMM_LED 6

#endif




