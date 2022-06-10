#include "./system.h" 

/**************************************************************************************/
/**************************************************************************************/
/*
 * 函数名：DS18B20_GPIO_Config
 * 描述  ：配置DS18B20用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
static void DS18B20_GPIO_Config(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 	
        
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
    GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 
}

/*
 * 函数名：DS18B20_Mode_IPU
 */
static void DS18B20_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}


/*
 * 函数名：DS18B20_Mode_Out_PP
 */
static void DS18B20_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;										   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 *主机给从机发送复位脉冲
 */
static void DS18B20_Rst2(void)
{
	DS18B20_Mode_Out_PP();           /* 主机设置为推挽输出 */	
	DS18B20_DATA_OUT(LOW);
	delay_us(750);                   /* 主机至少产生480us的低电平复位信号 */
    DS18B20_DATA_OUT(HIGH);          /* 主机在产生复位信号后，需将总线拉高 */
	delay_us(15);                  	 /*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
}

/*
 * 检测从机给主机返回的存在脉冲
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	DS18B20_Mode_IPU();/* 主机设置为上拉输入 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=100 )/* 经过100us后，存在脉冲都还没有到来*/
		return 1;
	else
		pulse_time = 0;
	while( !DS18B20_DATA_IN() && pulse_time<240 )/* 存在脉冲到来，且存在的时间不能超过240us */
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/*
 * 从DS18B20读取一个bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	DS18B20_Mode_Out_PP();/* 读0和读1的时间至少要大于60us */	
	DS18B20_DATA_OUT(LOW);/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	delay_us(10);
	DS18B20_Mode_IPU();/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	delay_us(2);
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	delay_us(45);/* 这个延时参数请参考时序图 */
	return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
uint8_t DS18B20_Read_Byte2(void)
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
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte2(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		
		if (testb)/* 写0和写1的时间至少要大于60us */
		{			
			DS18B20_DATA_OUT(LOW);		
			delay_us(8);/* 1us < 这个延时 < 15us */	
			DS18B20_DATA_OUT(HIGH);
			delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);	
			delay_us(70);	/* 60us < Tx 0 < 120us */
			DS18B20_DATA_OUT(HIGH);				
			delay_us(2);/* 1us < Trec(恢复时间) < 无穷大*/
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst2();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte2(0XCC);		/* 跳过 ROM */
	DS18B20_Write_Byte2(0X44);		/* 开始转换 */
}

uint8_t DS18B20_Init2(void)
{
	DS18B20_GPIO_Config();
	DS18B20_Rst2();
	return DS18B20_Presence();
}


short DS18B20_Get_Temp2(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	short f_tem;
	
	DS18B20_Rst2();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte2(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte2(0X44);				/* 开始转换 */
	
	DS18B20_Rst2();
    DS18B20_Presence();
	DS18B20_Write_Byte2(0XCC);				/* 跳过 ROM */
    DS18B20_Write_Byte2(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_Read_Byte2();		 
	tpmsb = DS18B20_Read_Byte2(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	f_tem = s_tem * 0.643;
	return f_tem; 	
}





void Get_Flow_Temp(void)
{
    float temp = 0.0f;
    static uint8_t i = 0;
//    static float sum = 0.0f,min = 0.0f,max = 0.0f;
     
    i++;
    temp = DS18B20_Get_Temp2();      
    if(i >= 10)
    {
        i = 10;
        pfout_temp = temp;
    }
}







