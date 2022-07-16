#include "led.h"


void LED_DC_Power(uint8_t sw)
{
    if(sw)
    {
        HAL_GPIO_WritePin(Machine_LED1_GPIO_Port, Machine_LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Machine_LED2_GPIO_Port, Machine_LED2_Pin, GPIO_PIN_SET);     /*Ãæ°åµÆ*/
    }
    else
    {
        HAL_GPIO_WritePin(Machine_LED1_GPIO_Port, Machine_LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Machine_LED2_GPIO_Port, Machine_LED2_Pin, GPIO_PIN_RESET);   /*Ãæ°åµÆ*/
    }
}


void LED_Alarm(uint8_t sw)
{
    if(sw)
    {
        HAL_GPIO_WritePin(Machine_LED5_GPIO_Port, Machine_LED5_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Machine_LED6_GPIO_Port, Machine_LED6_Pin, GPIO_PIN_RESET);     /*Ãæ°åµÆ*/    
    }
    else
    {
        HAL_GPIO_WritePin(Machine_LED5_GPIO_Port, Machine_LED5_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Machine_LED6_GPIO_Port, Machine_LED6_Pin, GPIO_PIN_RESET);   /*Ãæ°åµÆ*/    /*11 Y     01 R    00 close*/
    }
}


void LED_Comm_Toggle(void)
{
    HAL_GPIO_WritePin(Machine_LED3_GPIO_Port, Machine_LED3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_TogglePin(Machine_LED4_GPIO_Port,Machine_LED4_Pin);
}





