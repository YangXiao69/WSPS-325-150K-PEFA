#include "system.h"



void delay_us(uint16_t nCount)
{	
	uint32_t time = 0;
	time = nCount*72;
	for(; time != 0; time--);
}



void delay_ms(uint16_t nCount)
{	
	for(; nCount != 0; nCount--)
	delay_us(990);
}

void RcvSendBuffInit(void)
{
    CirBuff_Init(&cp);  
}


void Bsp_Inits(void)
{

	LED_INIT();
	Ctrl_GPIO_Init();	
    DS18B20_Init2();
    USART2_Config();
	I2C_EE_Init(); 

	read_from_24c02();
	ADC_Config();
	DAC_Config();	
		
    SPI_FLASH_Init() ;   //  W25Q64�ӿڳ�ʼ��
    
    
    
    
    w5500_init();
    socket_buf_init(txsize, rxsize);        /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/	
	
    w5500_top_init();
    socket_buf_init_top(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

	adc_data_calculate_static()             ;//����ADC������ֵ̬
    DAC_SetChannel2Data(DAC_Align_12b_R,500);
	DAC_SetChannel1Data(DAC_Align_12b_R,500);
	
	ADF4351_GPIO_Init();
	Generate_2450MHz_Freq();


    TIMx_Configuration();

    GetTotalRunTime();
    
    RcvSendBuffInit();


////    Empty_TimeLog();
}








