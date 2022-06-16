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


#include "./../bsp.h"   




__IO uint16_t g_usBoardTemp = 0;




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
    DS18B20_CLK_ENABLE(); 	

	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStruct.Pin = DS18B20_PIN;	
	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	/*������������Ϊ���� */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);	
    
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);	 
}





/*
 * ��������DS18B20_Mode_IPU
 * ����  ��ʹDS18B20-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/	
    GPIO_InitStruct.Pin = DS18B20_PIN;
    
    /*��������ģʽΪ����ģʽ*/ 
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT ; 
    
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    
    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
  
    /*���ÿ⺯������ʼ��DS18B20_PORT*/
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);	 
}

/*
 * ��������DS18B20_Mode_Out_PP
 * ����  ��ʹDS18B20-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DS18B20_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

	/*ѡ��Ҫ���Ƶ�DS18B20_PORT����*/															   
    GPIO_InitStruct.Pin = DS18B20_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   
  
    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	/*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 

	/*���ÿ⺯������ʼ��DS18B20_PORT*/
  	HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);	 	 
}


/*
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst(void)
{
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();
	
	DS18B20_DATA_OUT(DS18B20_LOW);
	
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
//	CPU_TS_Tmr_Delay_US(750);
    bsp_delayus(750);
	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_DATA_OUT(DS18B20_HIGH);
	
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
//	CPU_TS_Tmr_Delay_US(15);
    bsp_delayus(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
//		CPU_TS_Tmr_Delay_US(1);
        bsp_delayus(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !DS18B20_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
//		CPU_TS_Tmr_Delay_US(1);
        bsp_delayus(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/*
 * ��DS18B20��ȡһ��bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(DS18B20_LOW);
//	CPU_TS_Tmr_Delay_US(10);
	bsp_delayus(10);
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
//	CPU_TS_Tmr_Delay_US(2);
	bsp_delayus(2);
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
//	CPU_TS_Tmr_Delay_US(45);
	bsp_delayus(45);
	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(DS18B20_LOW);
			/* 1us < �����ʱ < 15us */
//			CPU_TS_Tmr_Delay_US(8);
			bsp_delayus(8);
			DS18B20_DATA_OUT(DS18B20_HIGH);
//			CPU_TS_Tmr_Delay_US(58);
            bsp_delayus(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(DS18B20_LOW);
			/* 60us < Tx 0 < 120us */
//			CPU_TS_Tmr_Delay_US(70);
			bsp_delayus(70);
			DS18B20_DATA_OUT(DS18B20_HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
//			CPU_TS_Tmr_Delay_US(2);
            bsp_delayus(5);
		}
	}
}

uint8_t DS18B20_Init(void)
{
	DS18B20_GPIO_Config();
	DS18B20_Rst();
	
	return DS18B20_Presence();
}
/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

 /**
  * @brief  ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
  * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
  * @retval ��
  */
void DS18B20_ReadId ( uint8_t * ds18b20_id )
{
	uint8_t uc = 0;
	
	
	DS18B20_Write_Byte(0x33);       //��ȡ���к�
	
	for ( uc = 0; uc < 8; uc ++ )
        ds18b20_id [ uc ] = DS18B20_Read_Byte();
	
}



void Get_Board_Temp(void)
{ 
    
    float temp = 0.0f;
    static uint8_t i = 0;
//    static float sum = 0.0f,min = 0.0f,max = 0.0f;

    i++;
    temp = DS18B20_Get_Temp();
    if(i >= 10)
    {
        i = 10;
        g_usBoardTemp = temp * 10;
    }
        
    
}




/*********************************************END OF FILE************************************/
