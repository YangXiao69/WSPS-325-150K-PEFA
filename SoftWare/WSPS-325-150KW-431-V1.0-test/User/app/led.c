#include "system.h"

    void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void PPT_SW_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void PPT_SW(u8 sw)
{
    if (sw)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        GPIO_SetBits(GPIOE, GPIO_Pin_0);
        GPIO_SetBits(GPIOC, GPIO_Pin_14);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        GPIO_ResetBits(GPIOE, GPIO_Pin_0);
        GPIO_ResetBits(GPIOC, GPIO_Pin_14);
    }
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
    if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3))
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    else
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
}
