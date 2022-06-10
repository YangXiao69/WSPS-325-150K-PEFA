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

#include "system.h"

#define ADC1_DR_Address ((u32)0x40012400 + 0x4c)

vu16 Current[4];
u16 VOLT_DEC;
float Pout;
float Pfout;
float Pin;

vu16 ADC_Value_Curr[5];  /* DMA数据保存 */
vu16 ADC_Value_Power[4]; /* DMA数据保存 */
u8 ADC_CUR_OK = 0;
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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
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

void ADC1_Mode_Config_ForCurr(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                 //ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Curr;                //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 5;                                       //1 DMA通道的DMA缓存的大小
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
    ADC_InitStructure.ADC_NbrOfChannel = 5;                             //要转换的通道数目
    ADC_Init(ADC1, &ADC_InitStructure);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); /*配置ADC时钟，为PCLK2的8分频，即9MHz*/

    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_71Cycles5); //VOLT_DEC
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 3, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 4, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 5, ADC_SampleTime_71Cycles5);

    ADC_DMACmd(ADC1, ENABLE);   /* Enable ADC1 DMA */
    ADC_Cmd(ADC1, ENABLE);      /* Enable ADC1 */
    ADC_ResetCalibration(ADC1); /*复位校准寄存器 */
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                       /*等待校准寄存器复位完成 */
    ADC_StartCalibration(ADC1); /* ADC校准 */
    while (ADC_GetCalibrationStatus(ADC1))
        ;                                   /* 等待校准完成*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
}

void ADC1_Mode_Config_ForPower(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                 //ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value_Power;               //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 4;                                       //1 DMA通道的DMA缓存的大小
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
    ADC_InitStructure.ADC_NbrOfChannel = 4;                             //要转换的通道数目
    ADC_Init(ADC1, &ADC_InitStructure);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); /*配置ADC时钟，为PCLK2的8分频，即9MHz*/

    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_7Cycles5); //OUT1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_7Cycles5); //OUT2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 3, ADC_SampleTime_7Cycles5);  //OUT3
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 4, ADC_SampleTime_7Cycles5);  //OUT4
    
    ADC_DMACmd(ADC1, ENABLE);   /* Enable ADC1 DMA */
    ADC_Cmd(ADC1, ENABLE);      /* Enable ADC1 */
    ADC_ResetCalibration(ADC1); /*复位校准寄存器 */
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                       /*等待校准寄存器复位完成 */
    ADC_StartCalibration(ADC1); /* ADC校准 */
    while (ADC_GetCalibrationStatus(ADC1))
        ;                                   /* 等待校准完成*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
}

/*
 * 函数名：ADC1_Init
 */
void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_Mode_Config_ForPower();
}

void ADC_for_Power(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    ADC1_Mode_Config_ForPower();
}
void ADC_for_CUR(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    ADC1_Mode_Config_ForCurr();
}

void CURR_calculate(void)
{
    uint8_t i;
    float date;
    static uint8_t t = 0;
    static vu16 max[5] = {0}, min[5] = {0}, sum[5] = {0};

    if (t == 0)
    {
        for (i = 0; i < 5; i++)
        {
            sum[i] = 0;
            max[i] = ADC_Value_Curr[i];
            min[i] = ADC_Value_Curr[i];
        }
    }
    for (i = 0; i < 5; i++)
    {
        sum[i] += ADC_Value_Curr[i];
        if (max[i] <= ADC_Value_Curr[i]) /* 保存最大值 */
            max[i] = ADC_Value_Curr[i];

        if (min[i] >= ADC_Value_Curr[i]) /* 保存最小值 */
            min[i] = ADC_Value_Curr[i];
    }
    t++;
    if (t == 4)
    {
        ADC_for_Power();
        sum[0] -= min[0] + max[0];
        date = sum[0] >> 1;
        VOLT_DEC = date / 4096 * 56.52;

        for (i = 1; i < 5; i++)
        {
            sum[i] -= min[i] + max[i];
            date = sum[i] >> 1;
            Current[i - 1] = date * 3.3 / 4096 * 80;
        }
        t = 0;
        ADC_CUR_OK = 1;

        error_serve();
    }
}

/*
1    430
2    470
3    425
4    475
*/
void adc_data(u16 num1, u16 num2, u16 num3)
{
    float data1, data2, data3;
    ADC_for_CUR();

    if (num1 > 54)
        data1 = num1 - 54;
    else
        data1 = 0;
    if (num2 > 54)
        data2 = num2 - 54;
    else
        data2 = 0;
    if (num3 > 180)
        data3 = num3 - 180;
    else
        data3 = 0;


    Pout = (0.0117 * data1 * data1 + 0.4095 * data1 + 30.967) * K_value / 1000;
    
    Pfout = (0.0117 * data2 * data2 + 0.4095 * data2 + 30.967) * 1000 / 1000;
    
    if (data3 > 1000)
        Pin = (0.00036 * data3 * data3 + 0.113 * data3) * 1000 / 20000;
    else
        Pin = (0.00042 * data3 * data3 + 0.053 * data3) * 1 / 20000;
    
    if(data1 == 0)
    {
        Pout = 0;
    }
    
}

void POWER_calculate(void)
{
    uint16_t data1 = 0, data2 = 0, data3 = 0;
    static uint16_t t = 0,i = 0;
    static uint16_t power_max = 0, powerR_max = 0;
    static uint32_t powerR_add = 0,power_add = 0;
    

    if(power_max < ADC_Value_Power[1])
    {
        power_max = ADC_Value_Power[1];
    }
    if(powerR_max < ADC_Value_Power[0])
    {
        powerR_max = ADC_Value_Power[0];
    }
    t++;
    if(t == 2000)
    {   
        i++;
          
        power_add += power_max;
        powerR_add += powerR_max;        
        if(i >= 8)
        {                               
            data1 = power_add/8;	/*正向功率*/                                               
            data2 = powerR_add/8;	/*内部反射功率*/
            data3 = 0;			    /*外部环形器检波，此型号不支持*/
            
            adc_data(data1, data2, data3);
            
            power_max = 0;
            powerR_max = 0;
            powerR_add = 0;
            power_add = 0;

            i = 0; 
            ADC_CUR_OK = 0;            
        }
        t = 0;
    }
    
    
    
}

//void POWER_calculate(void)
//{
//	  static uint16_t max1=0,max2=0;
//    static uint16_t t = 0,t0=0;
//	  static u8 start = 0;
//    static u16 num1,num2;
//
//	  if(start)
//		{
//				if(max1<ADC_Value_Power[0])max1=ADC_Value_Power[0];
//				if(max2<ADC_Value_Power[1])max2=ADC_Value_Power[1];
//
//				t++;
//				if(t == 12000)
//				{
//					 t = 0; max1=0; max2=0; t0=0;
//					 adc_data(max1,max2);
//					 ADC_CUR_OK = 0;
//					 start=0;
//				}
//		}
//		else
//		{
//				if(ADC_Value_Power[0]>200)
//				{
//						t0=0; t=0;
//						start=1;
//				}
//        else
//				{
//            t0++;
//					  if(t0==60000)
//						{
//							  ADC_CUR_OK = 0;
//						    ADC_AverageValue[7]=0;
//							  ADC_AverageValue[8]=0;
//							  adc_data(0,0);t0=0;
//						}
//				}
//		}
//}
