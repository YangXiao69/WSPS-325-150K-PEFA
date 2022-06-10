#include "./system.h" 

/**************************************************************************************/
/**************************************************************************************/
/*
 * ��������DS18B20_GPIO_Config
 * ����  ������DS18B20�õ���I/O��
 * ����  ����
 * ���  ����
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
 * ��������DS18B20_Mode_IPU
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
 * ��������DS18B20_Mode_Out_PP
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
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst2(void)
{
	DS18B20_Mode_Out_PP();           /* ��������Ϊ������� */	
	DS18B20_DATA_OUT(LOW);
	delay_us(750);                   /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    DS18B20_DATA_OUT(HIGH);          /* �����ڲ�����λ�źź��轫�������� */
	delay_us(15);                  	 /*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
}

/*
 * ���ӻ����������صĴ�������
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	DS18B20_Mode_IPU();/* ��������Ϊ�������� */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=100 )/* ����100us�󣬴������嶼��û�е���*/
		return 1;
	else
		pulse_time = 0;
	while( !DS18B20_DATA_IN() && pulse_time<240 )/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
 * ��DS18B20��ȡһ��bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	DS18B20_Mode_Out_PP();/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_DATA_OUT(LOW);/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	delay_us(10);
	DS18B20_Mode_IPU();/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	delay_us(2);
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	delay_us(45);/* �����ʱ������ο�ʱ��ͼ */
	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
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
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte2(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		
		if (testb)/* д0��д1��ʱ������Ҫ����60us */
		{			
			DS18B20_DATA_OUT(LOW);		
			delay_us(8);/* 1us < �����ʱ < 15us */	
			DS18B20_DATA_OUT(HIGH);
			delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);	
			delay_us(70);	/* 60us < Tx 0 < 120us */
			DS18B20_DATA_OUT(HIGH);				
			delay_us(2);/* 1us < Trec(�ָ�ʱ��) < �����*/
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst2();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte2(0XCC);		/* ���� ROM */
	DS18B20_Write_Byte2(0X44);		/* ��ʼת�� */
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
	DS18B20_Write_Byte2(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte2(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst2();
    DS18B20_Presence();
	DS18B20_Write_Byte2(0XCC);				/* ���� ROM */
    DS18B20_Write_Byte2(0XBE);				/* ���¶�ֵ */
	
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







