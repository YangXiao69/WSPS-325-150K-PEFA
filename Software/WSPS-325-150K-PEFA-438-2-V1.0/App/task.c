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



void Set_GPIO_Outmode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};   
    
    GPIO_InitStruct.Pin = P_D13_Pin|P_D14_Pin|P_D15_Pin|P_D0_Pin
                          |P_D1_Pin|P_D2_Pin|P_D3_Pin|P_D4_Pin
                          |P_D5_Pin|P_D6_Pin|P_D7_Pin|P_D8_Pin
                          |P_D9_Pin|P_D10_Pin|P_D11_Pin|P_D12_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(P_D5_GPIO_Port, &GPIO_InitStruct);


}

void Set_GPIO_InMode(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = P_D0_Pin|P_D1_Pin|P_D2_Pin|P_D3_Pin
                          |P_D4_Pin|P_D5_Pin|P_D6_Pin|P_D7_Pin
                          |P_D8_Pin|P_D9_Pin|P_D10_Pin|P_D11_Pin
                          |P_D12_Pin|P_D13_Pin|P_D14_Pin|P_D15_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(P_D5_GPIO_Port, &GPIO_InitStruct);


}

void Task_Ads8411_Send_Data(void)
{
    GPIOC->ODR = System.Pout;
    
    
}

