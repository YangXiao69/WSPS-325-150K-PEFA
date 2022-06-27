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
    GPIO_InitStruct.GPIO_Pin = DS18B20_PIN;
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    /*������������Ϊ���� */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);


    GPIO_WriteBit(DS18B20_PORT, DS18B20_PIN, Bit_SET);
}

/*********************************************END OF FILE************************************/
