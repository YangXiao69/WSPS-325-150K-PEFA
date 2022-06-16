#include "bsp_ds18b20.h"

/**
  **************************************************************************************
  * @brief  配置DS18B20用到的I/O口
  * @param  无
  * @retval 无
  **************************************************************************************  
  */
void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    /*设置引脚速率为高速 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStruct);  

    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStruct);      

    GPIO_WriteBit(DS18B20_PORT, DS18B20_PIN, Bit_SET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_5|GPIO_Pin_6, Bit_SET);
}

/*********************************************END OF FILE************************************/
