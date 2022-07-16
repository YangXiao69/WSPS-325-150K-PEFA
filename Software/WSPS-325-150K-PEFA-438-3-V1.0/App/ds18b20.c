#include "bsp_ds18b20.h"
#include "task.h"
#include "ds18b20.h"

/*
 * 函数名：DS18B20_Mode_IPU
 * 描述  ：使DS18B20-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void DS18B20_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*选择要控制的DS18B20_PORT引脚*/
	GPIO_InitStruct.Pin = DS18B20_PIN;

	/*设置引脚模式为输入模式*/
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	GPIO_InitStruct.Pull = GPIO_NOPULL;

	/*设置引脚速率为高速 */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	/*调用库函数，初始化DS18B20_PORT*/
	HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
    
}

/*
 * 函数名：DS18B20_Mode_Out_PP
 * 描述  ：使DS18B20-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void DS18B20_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*选择要控制的DS18B20_PORT引脚*/
	GPIO_InitStruct.Pin = DS18B20_PIN;

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

	/*设置引脚为上拉模式*/
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	/*设置引脚速率为高速 */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	/*调用库函数，初始化DS18B20_PORT*/
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
 *主机给从机发送复位脉冲
 */
static void DS18B20_Rst(uint8_t i)
{
	/* 主机设置为推挽输出 */
	DS18B20_Mode_Out_PP();

	DS18B20_DATA_OUT(i,DS18B20_LOW);

	/* 主机至少产生480us的低电平复位信号 */
	delay_us(750);

	/* 主机在产生复位信号后，需将总线拉高 */
	DS18B20_DATA_OUT(i,DS18B20_HIGH);

	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	delay_us(15);
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
static uint8_t DS18B20_Presence(uint8_t i)
{
	uint8_t pulse_time = 0;

	/* 主机设置为上拉输入 */
	DS18B20_Mode_IPU();

	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while (DS18B20_DATA_IN(i) && pulse_time < 100)
	{
		pulse_time++;
		delay_us(1);
	}
	/* 经过100us后，存在脉冲都还没有到来*/
	if (pulse_time >= 100)
		return 1;
	else
		pulse_time = 0;

	/* 存在脉冲到来，且存在的时间不能超过240us */
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
 * 从DS18B20读取一个bit
 */
static uint8_t DS18B20_Read_Bit(uint8_t i)
{
	uint8_t dat;

	/* 读0和读1的时间至少要大于60us */
	DS18B20_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_DATA_OUT(i,DS18B20_LOW);
	delay_us(10);

	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU();
	delay_us(2);

	if (DS18B20_DATA_IN(i) == SET)
		dat = 1;
	else
		dat = 0;

	/* 这个延时参数请参考时序图 */
	delay_us(45);

	return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
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
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte(uint8_t a,uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();

	for (i = 0; i < 8; i++)
	{
		testb = dat & 0x01;
		dat = dat >> 1;
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{
			DS18B20_DATA_OUT(a,DS18B20_LOW);
			/* 1us < 这个延时 < 15us */
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
			/* 1us < Trec(恢复时间) < 无穷大*/
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
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
float DS18B20_Get_Temp(uint8_t i)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;

	DS18B20_Rst(i);
	DS18B20_Presence(i);
	DS18B20_Write_Byte(i,0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(i,0X44); /* 开始转换 */

	DS18B20_Rst(i);
	DS18B20_Presence(i);
	DS18B20_Write_Byte(i,0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(i,0XBE); /* 读温度值 */

	tplsb = DS18B20_Read_Byte(i);
	tpmsb = DS18B20_Read_Byte(i);

	s_tem = tpmsb << 8;
	s_tem = s_tem | tplsb;

	if (s_tem < 0) /* 负温度 */
		f_tem = (~s_tem + 1) * 0.0625;
	else
		f_tem = s_tem * 0.0625;

	return f_tem;
}

/**
  * @brief  在匹配 ROM 情况下获取 DS18B20 温度值 
  * @param  ds18b20_id：用于存放 DS18B20 序列号的数组的首地址
  * @retval 无
  */
void DS18B20_ReadId(uint8_t *ds18b20_id)
{
	uint8_t uc = 0;

	DS18B20_Write_Byte(1,0x33); //读取序列号

	for (uc = 0; uc < 8; uc++)
		ds18b20_id[uc] = DS18B20_Read_Byte(1);
}


/*********************************************END OF FILE**********************/
