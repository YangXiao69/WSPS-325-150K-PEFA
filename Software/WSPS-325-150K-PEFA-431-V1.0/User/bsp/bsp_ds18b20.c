#include "bsp_ds18b20.h"

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    /*������������Ϊ���� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStruct);  

    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStruct);      

    GPIO_WriteBit(DS18B20_PORT, DS18B20_PIN, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_5|GPIO_Pin_6, Bit_SET);
}

/*********************************************END OF FILE************************************/
