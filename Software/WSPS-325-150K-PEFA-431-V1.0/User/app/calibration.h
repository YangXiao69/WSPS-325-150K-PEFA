#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#include "stm32f10x.h"


struct sSystem
{
    uint8_t Addr;
    uint8_t BasicVF; /* 正向基础电压 */
    uint8_t BasicVR; /* 反向基础电压 */
    uint16_t K_value;
    uint16_t K_value2;
    uint16_t K_value3;
    uint16_t temp[9];
    uint16_t Current[4];
    uint16_t Pin;
    uint16_t Pout;
    uint16_t Pfout;
    uint16_t Voltage;
    uint8_t RF[4];
    uint16_t Temp;
};

extern struct sSystem System;


void Get_Temp(void);
#endif




