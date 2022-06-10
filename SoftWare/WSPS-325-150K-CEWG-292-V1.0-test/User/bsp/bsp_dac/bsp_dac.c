#include "./bsp_dac.h"

/**
  * @brief  使能DAC的时钟，初始化GPIO
  * @param  无
  * @retval 无
  */
void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	   /* 使能GPIOA时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);  	   /* 使能DAC时钟 */	
    
    /* DAC的GPIO配置，模拟输入 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;						     //不使用触发
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	 //不使用波形发生器
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	   //不使用DAC输出缓冲
        
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);     /* 配置DAC 通道1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);                  /* 使能通道1 由PA4输出 */	
        
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);   /* 配置DAC 通道2 */
    DAC_Cmd(DAC_Channel_2, ENABLE);                /* 使能通道2 由PA5输出 */
    
    DAC_SetChannel2Data(DAC_Align_12b_R,0);
	DAC_SetChannel1Data(DAC_Align_12b_R,0);
}


