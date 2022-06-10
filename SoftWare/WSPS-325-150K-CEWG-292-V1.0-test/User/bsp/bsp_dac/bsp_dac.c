#include "./bsp_dac.h"

/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	   /* ʹ��GPIOAʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);  	   /* ʹ��DACʱ�� */	
    
    /* DAC��GPIO���ã�ģ������ */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;						     //��ʹ�ô���
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	 //��ʹ�ò��η�����
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	   //��ʹ��DAC�������
        
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);     /* ����DAC ͨ��1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);                  /* ʹ��ͨ��1 ��PA4��� */	
        
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);   /* ����DAC ͨ��2 */
    DAC_Cmd(DAC_Channel_2, ENABLE);                /* ʹ��ͨ��2 ��PA5��� */
    
    DAC_SetChannel2Data(DAC_Align_12b_R,0);
	DAC_SetChannel1Data(DAC_Align_12b_R,0);
}


