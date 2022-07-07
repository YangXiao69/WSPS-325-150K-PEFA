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
  * @brief  配置DS18B20用到的I/O口
  * @param  无
  * @retval 无
  **************************************************************************************  
  */
void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DS18B20_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    
    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = DS18B20_PIN;
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    /*设置引脚速率为高速 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);  

    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);      

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);

}

/*********************************************END OF FILE************************************/
