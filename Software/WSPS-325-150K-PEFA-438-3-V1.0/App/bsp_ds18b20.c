/**
  **************************************************************************************
  * @file       bsp_ds18b20.c
  * @author  
  * @version    V1.00
  * @date       2021-01-07
  * @brief    
  * @attention
  *
  * 
  * 
  * 
  *
  **************************************************************************************
  */

#include "./bsp_ds18b20.h"

/**
  **************************************************************************************
  * @brief  ����DS18B20�õ���I/O��
  * @param  ��
  * @retval ��
  **************************************************************************************  
  */
void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DS18B20_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.Pin = DS18B20_PIN;
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    /*������������Ϊ���� */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);      

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

}

/*********************************************END OF FILE************************************/
