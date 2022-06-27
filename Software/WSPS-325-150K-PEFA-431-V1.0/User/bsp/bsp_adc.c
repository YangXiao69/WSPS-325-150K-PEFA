/*
*  通道对应关系
  PA_0  ADC_Channel_0    SENSOR_1
	PA_1  ADC_Channel_1	   SENSOR_2
	PA_2  ADC_Channel_2    SENSOR_3
	PA_3  ADC_Channel_3    SENSOR_4
	PA_4  ADC_Channel_4
	PA_5  ADC_Channel_5
	PA_6  ADC_Channel_6	   V+
	PA_7  ADC_Channel_7    V-
	PB_0  ADC_Channel_8    V入
	PB_1  ADC_Channel_9    OUT3
	PC_0  ADC_Channel_10
	PC_1  ADC_Channel_11
	PC_2  ADC_Channel_12
	PC_3  ADC_Channel_13   VOLT_DEC
	PC_4  ADC_Channel_14   OUT1
	PC_5  ADC_Channel_15   OUT2

	DMA通道
	DMA1_Channel1
*/

#include "bsp.h"

#define ADC1_DR_Address ((u32)0x40012400 + 0x4c)

uint16_t  ADC_Value_Power[9]; /* DMA数据保存 */

/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);

    /* GPIOA */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOB */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOC */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}



void ADC1_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                 //ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Power;               //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 9;                                       //1 DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存地址寄存器递增  //内存地址固定 DMA_MemoryInc_Disable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE); /* Enable DMA channel1 */

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        //使能扫描模式，扫描模式用于多通道采集 DISABLE
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  //开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不使用外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 9;                             //要转换的通道数目
    ADC_Init(ADC1, &ADC_InitStructure);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); /*配置ADC时钟，为PCLK2的8分频，即9MHz*/

    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_7Cycles5); //OUT1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_7Cycles5); //OUT2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 3, ADC_SampleTime_7Cycles5);  //OUT3
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_7Cycles5);  //OUT4
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13,5, ADC_SampleTime_7Cycles5); //VOLT_DEC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 6, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 7, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 8, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 9, ADC_SampleTime_7Cycles5);
    
    ADC_DMACmd(ADC1, ENABLE);   /* Enable ADC1 DMA */
    ADC_Cmd(ADC1, ENABLE);      /* Enable ADC1 */
    ADC_ResetCalibration(ADC1); /*复位校准寄存器 */
    while (ADC_GetResetCalibrationStatus(ADC1));                       /*等待校准寄存器复位完成 */
    ADC_StartCalibration(ADC1); /* ADC校准 */
    while (ADC_GetCalibrationStatus(ADC1));                                   /* 等待校准完成*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
}

/*
 * 函数名：ADC1_Init
 */
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config();
}

