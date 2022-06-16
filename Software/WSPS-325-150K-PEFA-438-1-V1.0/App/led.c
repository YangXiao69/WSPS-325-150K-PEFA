#include "led.h"


void LED_DC_Power(uint8_t sw)
{
    if(sw)
    {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);     /*Ãæ°åµÆ*/
    }
    else
    {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);   /*Ãæ°åµÆ*/
    }
}


void LED_Comm_Toggle(void)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_5);
}





