#ifndef __TASK_H__
#define __TASK_H__

#include "main.h"
#include "ads8411.h"

struct sSystem
{
    uint16_t Pout;
    uint16_t RPout;
};


void delay_10ns(uint8_t i);
void delay_ms(uint8_t i);
void  Task_Get_Ads8411_Data(void);
void Task_Send_Data(void);
#endif







