#include "system.h" 


__IO uint16_t ADC_ConvertedValue[5]={0};

__IO float ADC_AverageValue;	  /* 多次采样求取平均值 */

float PowerOut,PowerR,PowerZB;
float PfoutOut;
float Flow,FlowTemp = 0.0;
float pfout_temp = 0;
u16 V_data,V_data1,V_data2;


unsigned char OPD_Static_Data = 0;
unsigned char RPD_Static_Data = 0;
unsigned char ANR_Static_Data = 0;


static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);				
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |  GPIO_Pin_4 |  GPIO_Pin_5 | GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

static void ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
    // ------------------DMA Init 结构体参数 初始化--------------------------
    // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的

    RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);                   // 开启ADC时钟
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;		 // 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;     // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	             // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_BufferSize = 5;	                               // 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             // 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      // 存储器地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	         //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响// 循环传输模式
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;             // 禁止DMA FIFO	，使用直连模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  // FIFO 大小，FIFO模式禁止时，这个不用配置	
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
    DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL;     // 选择 DMA 通道，通道存在于流中
    DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);        //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
    DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);// 使能DMA流
	
    // -------------------ADC Common 结构体 参数 初始化------------------------
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                       // 独立ADC模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                    // 时钟为fpclk x分频	
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        // 禁止DMA直接访问模式	
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 采样时间间隔	
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    // -------------------ADC Init 结构体 参数 初始化--------------------------
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                       // ADC 分辨率
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                 // 扫描模式，多通道采集需要	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                           // 连续转换	 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  //禁止外部边沿触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;        //使用软件触发，外部触发不用配置，注释掉即可
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                       //数据右对齐	
    ADC_InitStructure.ADC_NbrOfConversion = 5;                                   //转换通道                      
    ADC_Init(ADC1, &ADC_InitStructure);
    
// 配置 ADC 通道转换顺序和采样时间周期
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_56Cycles);
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_56Cycles); 
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  3, ADC_SampleTime_56Cycles);
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_56Cycles);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_56Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  2, ADC_SampleTime_56Cycles); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 3, ADC_SampleTime_56Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_56Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_56Cycles);
    
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); // 使能DMA请求 after last transfer (Single-ADC mode)
    ADC_DMACmd(ADC1, ENABLE);    // 使能ADC DMA
    ADC_Cmd(ADC1, ENABLE);       // 使能ADC
    ADC_SoftwareStartConv(ADC1); // 开始adc转换，软件触发
}


void ADC_Config(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
}

void Alarm_Led(void)
{
    static u8 time=0;
	
    time++;
	 
    if(time>30)
    {
        time=0;//BEER_ON();	
        if(!GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_6)) LED_CONTROL(ALARM_RED_LED); 
        else LED_CONTROL(ALARM_OFF_LED); 				
    }
}

void adc_data(u16 d1,u16 d2,u16 d3)
{   
//    static u8 errorPout=0,errorPfout=0,errorPr=0,errorZhubo=0;
	float num1 = 0.0f, num2 = 0.0f;
	  
//    RF = 1;
    if(RF && Pset)
    { 
        if(d1 <= OPD_Static_Data)
        {
            d1 = 0;
        }
        else
        {
            d1 = d1 - OPD_Static_Data+2;	
        }
    
    
        Modbus[0x0a] = d1;
        
        
        

        PowerOut = (0.016*d1*d1+0.3259*d1+140.53)* V_data/1000; 
        
        if(d1 == 0)
            PowerOut = 0;
        
        
        if(PowerOut < 0 || d1 <= 0)  
            PowerOut = 0;
        
        
        
        if(d2 <= RPD_Static_Data)
        {
            d2 = 0;		
        }
        else
        {
            d2 = d2 - RPD_Static_Data;	
        
        }
            
        PowerR  = (0.016*d2*d2 + 0.3259*d2+140.53)* V_data1/1000; 
        
        if(PowerR < 0 || d2 <= 0)  
            PowerR = 0;
        
        
           
        num1 = sqrt(PowerOut) + sqrt(PowerR);
        num2 = sqrt(PowerOut) - sqrt(PowerR);
        
        
        if(num2 > 0)
        {
            PowerZB = num1/num2*10;
        }
        else
        {
            PowerZB = 999;
        }
        
        if(PowerZB>999)
        {
            PowerZB=999;
        }
        
        if(PowerZB < 10)
        {
            PowerZB = 10;
        
        }  

        if(Pset <= 50)  PowerZB = 10;


        
    }
    else
    {
        PowerOut = 0;
        PowerR = 0;
        PowerZB = 10;
    
    }

    if(RF)
    {    
        if(d3 <= ANR_Static_Data)
        {
            d3 = 0;		
        }
        else
        {
            d3 = d3 - ANR_Static_Data;	
        }
        
        PfoutOut = (0.000657*d3*d3 + 0.11205*d3-83.28859)* V_data2/3000;
                
        if(PfoutOut < 0 || d3 <= 100) 
            PfoutOut = 0;
	}
    else
    {
        PfoutOut = 0;
    }
    
//    DAC_SetChannel2Data(DAC_Align_12b_R,Pset);	   		 
//    DAC_SetChannel1Data(DAC_Align_12b_R,1300);    
    
    Power_Adj();	    
}




void adc_data_calculate(void)
{
    u16 data1=0,data2=0,data3=0;
	float adcflow = 0.0,flowtemp_v;
    static u8 t =0;
//    static u8 errorFlow = 0,errorFlowTemp = 0;
    static u32 num1,num2,num3,sum = 0,min = 0,max = 0;

	num1 += ADC_ConvertedValue[0];
	num2 += ADC_ConvertedValue[1];	
	num3 += ADC_ConvertedValue[2];	
		
    t++;
	if(t == 64)
	{ 
        data1 = num1>>6;
        data2 = num2>>6;
        data3 = num3>>6;			  			
        adc_data(data1,data2,data3);			
        num1 = num2 = num3 =0;
	}	
		
	if(t == 1)
	{			
        sum = 0;
        max = ADC_ConvertedValue[3];
        min = ADC_ConvertedValue[3];				
	}
		

    sum += ADC_ConvertedValue[3];
    if(max <= ADC_ConvertedValue[3]) 	  /* 保存最大值 */
        max = ADC_ConvertedValue[3];
    
    if(min >= ADC_ConvertedValue[3]) 	  /* 保存最小值 */
        min = ADC_ConvertedValue[3];			   
    
    if(t == 64)
    {
        t = 0;
        sum -= (min + max);                     /* 除去最大值和最小值 */
        ADC_AverageValue = (float)(sum / 62);		  
            
        adcflow = (float)(ADC_AverageValue * 3.31f/4096);
        Flow = (float)(adcflow  * 41.036f - 26.764f)*10;
        if(Flow < 0)
        {
            Flow = 0;
        }

        flowtemp_v = (uint16_t)ADC_ConvertedValue[4];
        FlowTemp = (flowtemp_v * (3.31f/4096));
        
        FlowTemp = (float)(FlowTemp  * 41.036f - 26.764f)*10;;
    }		
}

void adc_data_calculate_static(void)
{	
    static u8 t = 0;
    static u32 num1,num2,num3;				
    for(t=0;t<64;t++)
    { 
        num1 += ADC_ConvertedValue[0];
        num2 += ADC_ConvertedValue[1];	
        num3 += ADC_ConvertedValue[2];
        delay_us(10);
    }
    OPD_Static_Data = num1>>6;if (OPD_Static_Data>200){OPD_Static_Data = 200;}
    RPD_Static_Data = num2>>6;if (RPD_Static_Data>200){RPD_Static_Data = 200;}
    ANR_Static_Data = num3>>6;if (ANR_Static_Data>200){ANR_Static_Data = 200;}				
    t = 0;  					
    num1 = num2 = num3 =0;
}


