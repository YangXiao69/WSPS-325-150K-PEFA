#ifndef __TASK_H__
#define __TASK_H__

#include "main.h"
#include "ads8411.h"

struct sSystem
{
    uint16_t Pout;
    uint16_t RPout;
};

void delay_10ns(uint16_t nCount);
void delay_us( uint32_t time_us );
void delay_ms(uint8_t time_ms);
void Task_Get_Ads8411_Data(void);
void Task_Ads8411_Send_Data(void);
#endif







