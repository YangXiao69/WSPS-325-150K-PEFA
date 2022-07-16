#include "bsp_ds18b20.h"
#include "task.h"
#include "ds18b20.h"

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
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

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

void DS18B20_DATA_OUT(uint8_t i,GPIO_PinState value)
{
    if(i == 1)
    {
        HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, value);
    }
    else if(i == 2)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, value);
    }
    else if(i == 3)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, value);
    }    
    else if(i == 4)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, value);
    }    
}


uint8_t DS18B20_DATA_IN(uint8_t i)
{
    uint8_t value;
    if(i == 1)
    {
        value = HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);
    }
    else if(i == 2)
    {
        value = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
    }  
    else if(i == 3)
    {
        value = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
    }     
    else if(i == 4)
    {
        value = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
    } 
    return value;
}

/*
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst(uint8_t i)
{
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();

	DS18B20_DATA_OUT(i,DS18B20_LOW);

	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	delay_us(750);

	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_DATA_OUT(i,DS18B20_HIGH);

	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	delay_us(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
static uint8_t DS18B20_Presence(uint8_t i)
{
	uint8_t pulse_time = 0;

	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();

	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while (DS18B20_DATA_IN(i) && pulse_time < 100)
	{
		pulse_time++;
		delay_us(1);
	}
	/* ����100us�󣬴������嶼��û�е���*/
	if (pulse_time >= 100)
		return 1;
	else
		pulse_time = 0;

	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while (!DS18B20_DATA_IN(i) && pulse_time < 240)
	{
		pulse_time++;
		delay_us(1);
	}
	if (pulse_time >= 240)
		return 1;
	else
		return 0;
}

/*
 * ��DS18B20��ȡһ��bit
 */
static uint8_t DS18B20_Read_Bit(uint8_t i)
{
	uint8_t dat;

	/* ��0�Ͷ�1��ʱ������Ҫ����60us */
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(i,DS18B20_LOW);
	delay_us(10);

	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	delay_us(2);

	if (DS18B20_DATA_IN(i) == SET)
		dat = 1;
	else
		dat = 0;

	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(45);

	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20_Read_Byte(uint8_t a)
{
	uint8_t i, j, dat = 0;

	for (i = 0; i < 8; i++)
	{
		j = DS18B20_Read_Bit(a);
		dat = (dat) | (j << i);
	}

	return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte(uint8_t a,uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();

	for (i = 0; i < 8; i++)
	{
		testb = dat & 0x01;
		dat = dat >> 1;
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{
			DS18B20_DATA_OUT(a,DS18B20_LOW);
			/* 1us < �����ʱ < 15us */
			delay_us(8);

			DS18B20_DATA_OUT(a,DS18B20_HIGH);
			delay_us(58);
		}
		else
		{
			DS18B20_DATA_OUT(a,DS18B20_LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);

			DS18B20_DATA_OUT(a,DS18B20_HIGH);
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(2);
		}
	}
}

uint8_t DS18B20_Init(void)
{
	DS18B20_GPIO_Config();
	DS18B20_Rst(1);
    DS18B20_Presence(1);
    DS18B20_Rst(2);
    DS18B20_Presence(2);
    DS18B20_Rst(3);
    DS18B20_Presence(3);
    DS18B20_Rst(4);

	return DS18B20_Presence(4);
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
float DS18B20_Get_Temp(uint8_t i)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;

	DS18B20_Rst(i);
	DS18B20_Presence(i);
	DS18B20_Write_Byte(i,0XCC); /* ���� ROM */
	DS18B20_Write_Byte(i,0X44); /* ��ʼת�� */

	DS18B20_Rst(i);
	DS18B20_Presence(i);
	DS18B20_Write_Byte(i,0XCC); /* ���� ROM */
	DS18B20_Write_Byte(i,0XBE); /* ���¶�ֵ */

	tplsb = DS18B20_Read_Byte(i);
	tpmsb = DS18B20_Read_Byte(i);

	s_tem = tpmsb << 8;
	s_tem = s_tem | tplsb;

	if (s_tem < 0) /* ���¶� */
		f_tem = (~s_tem + 1) * 0.0625;
	else
		f_tem = s_tem * 0.0625;

	return f_tem;
}

/**
  * @brief  ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
  * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
  * @retval ��
  */
void DS18B20_ReadId(uint8_t *ds18b20_id)
{
	uint8_t uc = 0;

	DS18B20_Write_Byte(1,0x33); //��ȡ���к�

	for (uc = 0; uc < 8; uc++)
		ds18b20_id[uc] = DS18B20_Read_Byte(1);
}


/*********************************************END OF FILE**********************/
