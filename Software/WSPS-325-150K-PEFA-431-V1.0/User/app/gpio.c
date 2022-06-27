#include "gpio.h"



void Addr_Init(void)
{
    uint8_t addr = 0;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //  PC11 IO1 5     PC10 IO2 8     PA12 IO3 7
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) == 0)
        addr += 128;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0)
        addr += 64;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 0)
        addr += 32;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0)
        addr += 16;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0)
        addr += 8;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0)
        addr += 4;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) == 0)
        addr += 2;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15) == 0)
        addr += 1;
    if(addr == 0)
    {
        addr++;
    }

    System.Addr = addr;
}



void LED_ON(uint16_t name)
{
    switch (name)
    {
    case 1:
        GPIO_SetBits(GPIOE, GPIO_Pin_1);
        break;
    case 2:
        GPIO_SetBits(GPIOE, GPIO_Pin_2);
        break;
    case 3:
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
        break;
    case 4:
        GPIO_SetBits(GPIOE, GPIO_Pin_4);
        break;
    case 5:
        GPIO_SetBits(GPIOE, GPIO_Pin_5);
        break;
    case 6:
        GPIO_SetBits(GPIOE, GPIO_Pin_6);
        break;
    default:
        break;
    }
}

void LED_OFF(uint16_t name)
{
    switch (name)
    {
    case 1:
        GPIO_ResetBits(GPIOE, GPIO_Pin_1);
        break;
    case 2:
        GPIO_ResetBits(GPIOE, GPIO_Pin_2);
        break;
    case 3:
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);
        break;
    case 4:
        GPIO_ResetBits(GPIOE, GPIO_Pin_4);
        break;
    case 5:
        GPIO_ResetBits(GPIOE, GPIO_Pin_5);
        break;
    case 6:
        GPIO_ResetBits(GPIOE, GPIO_Pin_6);
        break;
    default:
        break;
    }
}

void LED_COML(void)
{
    if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_6))
        LED_OFF(COMM_LED);
    else
        LED_ON(COMM_LED);
}



