#include "task.h"

struct sSystem System;


void delay_10ns(uint16_t nCount)
{
    while(nCount--);
}


void delay_us( uint32_t time_us )
{
    uint32_t temp = 0;
    if (time_us > 39000)
    {
        time_us = 39000;
    }

    SysTick->LOAD = time_us * 72;                //ʱ�����
    SysTick->VAL = 0x00;                      //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL = 0x00;                       //��ռ�����    
    
}


void delay_ms(uint8_t time_ms)
{	 		  	  
    uint32_t temp = 0;
    if (time_ms > 39)
    {
    time_ms = 39;
    }

    SysTick->LOAD = (uint32_t)time_ms * 7200;    //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;                      //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
    do
    {
    temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //�رռ�����
    SysTick->VAL = 0x00;                            //��ռ�����   
}




void  Task_Get_Ads8411_Data(void)
{
    System.Pout = get_ADS8411Data_A();
    System.RPout = get_ADS8411Data_B();
}


void Task_Ads8411_Send_Data(void)
{
    GPIOC->ODR = System.Pout;
    
    
}

