#include "ads8411.h"


/*
   adsBUSY    PE6   ��ȡ״̬Ϊ 1 ʱ ��ʾ���ڽ���ת��
	 adsRESET   PE5   ��λ�͵�ƽ��Ч
	 adsBYTE    PE4   ���� 1 ʱ ��ȡ���ݴ�0-15λ��
	 adsCONVST  PE3   ��ʼת�� �͵�ƽ��Ч
	 adsCS      PB5   Ƭѡ �͵�ƽ��Ч
	 adsRD      PE2   ͬ���������ȡΪ 0 ʱ����ʾ��������ɶ�ȡ

	 ʹ�÷�ʽ��adsCSʼ�����ͣ�adsRD��Ϊ��ȡʹ�� adsBYTEĬ��Ϊ0��adsBYTEΪ1ʱ���ٴζ�ȡ15��8Ϊ7��0�����ݣ������Ų����õ������ʹ�ã�����ʹ�ã�

	 Timing for Conversion and Acquisition Cycles With CS Tied to BDGND, RD Toggling
*/



void adsCONVST_Start_A(void)
{
    while(HAL_GPIO_ReadPin(AD1_BUSY_GPIO_Port,AD1_BUSY_Pin));    //�ȴ� adsBUSY �ź���ʧ
    HAL_GPIO_WritePin(AD1_CS_GPIO_Port,AD1_CS_Pin,GPIO_PIN_RESET);  //adsCS = 0;
    HAL_GPIO_WritePin(AD1_BYTE_GPIO_Port,AD1_BYTE_Pin,GPIO_PIN_RESET);  //adsBYTE = 0;
    HAL_GPIO_WritePin(AD1_RD_GPIO_Port,AD1_RD_Pin,GPIO_PIN_RESET);  //adsRD = 0;
    HAL_GPIO_WritePin(AD1_CONVST_GPIO_Port,AD1_CONVST_Pin,GPIO_PIN_SET);   /*CONVST = 1*/
    delay_10ns(3);
    HAL_GPIO_WritePin(AD1_CONVST_GPIO_Port,AD1_CONVST_Pin,GPIO_PIN_RESET);   /*CONVST = 0*/
    while(HAL_GPIO_ReadPin(AD1_BUSY_GPIO_Port,AD1_BUSY_Pin));    //�ȴ� adsBUSY �ź���ʧ
}

void ads8411_reset_A(void)
{
    HAL_GPIO_WritePin(AD1_CS_GPIO_Port,AD1_CS_Pin,GPIO_PIN_RESET);  //adsCS = 0;
    HAL_GPIO_WritePin(AD1_RESET_GPIO_Port,AD1_RESET_Pin,GPIO_PIN_RESET);  //adsRESET = 0;
    delay_ms(10);
    HAL_GPIO_WritePin(AD1_RESET_GPIO_Port,AD1_RESET_Pin,GPIO_PIN_SET);    //adsRESET = 1;
}

uint16_t ads8411_Conversion_A(void)
{
    uint16_t data;
    adsCONVST_Start_A();

    delay_10ns(5);
    data = ((uint16_t)GPIOD->IDR);
    return data;
}

uint16_t get_ADS8411Data_A(void)
{
    uint16_t i=0, data=0;
    uint16_t MaxData=0;

    for(i=0; i<1000; i++)    // 1ms
    {
        data = ads8411_Conversion_A();
        if(MaxData < data)	MaxData = data;
    }
    return MaxData;
}

/***********************************************************************/

void adsCONVST_Start_B(void)
{
    while(HAL_GPIO_ReadPin(AD2_BUSY_GPIO_Port,AD2_BUSY_Pin));    //�ȴ� adsBUSY �ź���ʧ
    HAL_GPIO_WritePin(AD2_CS_GPIO_Port,AD2_CS_Pin,GPIO_PIN_RESET);  //adsCS = 0;
    HAL_GPIO_WritePin(AD2_BYTE_GPIO_Port,AD2_BYTE_Pin,GPIO_PIN_RESET);  //adsBYTE = 0;
    HAL_GPIO_WritePin(AD2_RD_GPIO_Port,AD2_RD_Pin,GPIO_PIN_RESET);  //adsRD = 0;
    HAL_GPIO_WritePin(AD2_CONVST_GPIO_Port,AD2_CONVST_Pin,GPIO_PIN_SET);   /*CONVST = 1*/
    delay_10ns(3);
    HAL_GPIO_WritePin(AD2_CONVST_GPIO_Port,AD2_CONVST_Pin,GPIO_PIN_RESET);   /*CONVST = 0*/
    while(HAL_GPIO_ReadPin(AD2_BUSY_GPIO_Port,AD2_BUSY_Pin));    //�ȴ� adsBUSY �ź���ʧ
}

void ads8411_reset_B(void)
{
    HAL_GPIO_WritePin(AD2_CS_GPIO_Port,AD2_CS_Pin,GPIO_PIN_RESET);  //adsCS = 0;
    HAL_GPIO_WritePin(AD2_RESET_GPIO_Port,AD2_RESET_Pin,GPIO_PIN_RESET);  //adsRESET = 0;
    delay_ms(10);
    HAL_GPIO_WritePin(AD2_RESET_GPIO_Port,AD2_RESET_Pin,GPIO_PIN_SET);    //adsRESET = 1;
}

uint16_t ads8411_Conversion_B(void)
{
    uint16_t data;
    adsCONVST_Start_B();

    delay_10ns(5);
    data = ((uint16_t)GPIOE->IDR);
    return data;
}

uint16_t get_ADS8411Data_B(void)
{
    uint16_t i=0, data=0;
    uint16_t MaxData=0;

    for(i=0; i<1000; i++)    // 1ms
    {
        data = ads8411_Conversion_B();
        if(MaxData < data)	MaxData = data;
    }
    return MaxData;
}
