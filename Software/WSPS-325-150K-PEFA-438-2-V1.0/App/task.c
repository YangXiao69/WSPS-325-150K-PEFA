#include "task.h"

struct sSystem System;



void delay_10ns(uint8_t i)
{
    
    
    
    
}


void delay_ms(uint8_t i)
{
    
    
    
    
}


void  Task_Get_Ads8411_Data(void)
{
    System.Pout = get_ADS8411Data_A();
    System.RPout = get_ADS8411Data_B();
}


void Task_Send_Data(void)
{
    
    
    
}

