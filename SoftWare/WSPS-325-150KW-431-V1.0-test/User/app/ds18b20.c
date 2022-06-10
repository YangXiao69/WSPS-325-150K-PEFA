/*
* 10· �¶����ݹܽŶ�Ӧ
*	GPIO_D0-7  GPIO_D15
*/
#include "ds18b20.h"
#include "utility.h"

#define DS18B20_DQ_OUT0 GPIO_ResetBits(GPIOD, 0X00FF) //�������ݽ���� 0
#define DS18B20_DQ_OUT1 GPIO_SetBits(GPIOD, 0X00FF)   //�������ݽ���� 1

#define AP_temp1 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) //��ȡ����
#define AP_temp2 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) //��ȡ����
#define AP_temp3 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) //��ȡ����
#define AP_temp4 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //��ȡ����
#define AP_temp5 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) //��ȡ����
#define AP_temp6 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //��ȡ����
#define AP_temp7 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) //��ȡ����
#define AP_temp8 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) //��ȡ����

int16_t temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; /* ������1-4   temp[0] temp[2] temp[4] temp[6]:
                                         ����  1-4   temp[1] temp[3] temp[5] temp[7]:
                                         �ϳ���      temp[8]  */

void DS18B20_IO_OUT(void) /* ���ݽ����ģʽ */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = 0X00FF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* 50Mʱ���ٶ� */
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void DS18B20_IO_IN(void) /* ���ݽ�����ģʽ */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = 0X00FF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/*
*  ��Ӧ�˿ڶ�ȡ
*  �����16λ�ֱ��Ӧ����λ
*/
u16 DQ_IN(void) /* ���ݽ������ȡ */
{
    u16 date;
    date = GPIO_ReadInputData(GPIOD) & 0X00FF;
    return date;
}

/* ��λDS18B20 */
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();
    GPIO_ResetBits(GPIOD, 0X00FF); //�������ݽ���� 0
    delay_us(750);                 //����750us
    GPIO_SetBits(GPIOD, 0X00FF);   //DQ=1
    delay_us(1);
}

/* �ȴ�DS18B20�Ļ�Ӧ������ֵΪ0X00FF ��ȫ���������쳣�򷵻��쳣ֵ */
u16 DS18B20_Check(void)
{
    u8 retry = 0;

    DS18B20_IO_IN();
    while (DQ_IN() && retry < 200) /* �ȴ�����͵�ƽ�������ȴ�200us��*/
    {
        retry++;
        delay_us(1);
    };
    if (retry >= 200)
        return 1;
    else
        retry = 0;
    while ((DQ_IN() != 0X00FF) && retry < 240) /* 10����ʱ�����ҪС��240us���͵�ƽ������ʱ�䣩 */
    {
        retry++;
        delay_us(1);
    };
    if (retry >= 240)
        return 1;
    return 0;
    //   u16 retry=0;
    //	 DS18B20_IO_IN();
}

/* ��ʼ��DS18B20��IO�ڣ� DQ ͬʱ���DS�Ĵ��ڣ�����1:�����ڡ�����0:���� */
u8 DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = 0X00FF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    DS18B20_Rst();
    return DS18B20_Check();
}

/* дһ���ֽڵ�DS18B20��ʮ��ͬʱд��ͬ�������� */
void DS18B20_Write_Byte(u8 dat)
{
    u8 j;
    u8 testb;
    DS18B20_IO_OUT(); //SET PG11 OUTPUT;
    for (j = 0; j < 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb)
        {
            DS18B20_DQ_OUT0; // Write 1
            delay_us(5);
            DS18B20_DQ_OUT1;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT0; // Write 0
            delay_us(60);
            DS18B20_DQ_OUT1;
            delay_us(5);
        }
    }
}

/* ��18B20��ȡһ���ֽ� */
uint8_t DS18B20_Read_Byte(u8 num)
{
    u8 i, j;
    uint8_t dat = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* 50Mʱ���ٶ� */

    switch (num)
    {
    case 0:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_7);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp1;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 1:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_6);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp2;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_5);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp3;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 3:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_4);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp4;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 4:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_3);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp5;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 5:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_2);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp6;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 6:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_1);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp7;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 7:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; /* ���ģʽ */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_0);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* �������� */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp8;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
        //		case 8: for(i=0; i<8; i++)
        //            {
        //								GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;            /* ���ģʽ */
        //								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        //								GPIO_Init(GPIOD, &GPIO_InitStructure);
        //								GPIO_ResetBits(GPIOD,GPIO_Pin_15);
        //								delay_us(2);
        //								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        /* �������� */
        //								GPIO_Init(GPIOD, &GPIO_InitStructure);
        //							  j = AP_temp8;
        //							  dat = (dat) | (j<<i);
        //						    delay_us(45);
        //						}	  break;
    }
    return dat;
}

/* ��ds18b20�õ�10���¶�ֵ���õ����¶�ֵ��������� temp[10] �� */
void DS18B20_Get_Temp(void)
{
    u8 i;
    uint8_t tpmsb, tplsb;
    short s_tem;

    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0XCC); /* ���� ROM */
    DS18B20_Write_Byte(0X44); /* ��ʼת�� */

    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0XCC); /* ���� ROM */
    DS18B20_Write_Byte(0XBE); /* ���¶�ֵ */

    for (i = 0; i < 8; i++)
    {
        tplsb = DS18B20_Read_Byte(i);
        tpmsb = DS18B20_Read_Byte(i);
        s_tem = tpmsb << 8;
        s_tem = s_tem | tplsb;
        if (s_tem < 0)                      /* ���¶� */
            temp[i] = (~s_tem + 1) * 0.625; /* �õ����¶�ֵΪ����������ʵ��ֵ�Ŵ�10�� */
        else
            temp[i] = s_tem * 0.625;
    }
}

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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 * ��������DS18B20_Mode_Out_PP
 */
static void DS18B20_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst2(void)
{
    DS18B20_Mode_Out_PP(); /* ��������Ϊ������� */
    DS18B20_DATA_OUT(LOW);
    delay_us(750);          /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    DS18B20_DATA_OUT(HIGH); /* �����ڲ�����λ�źź��轫�������� */
    delay_us(15);           /*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
}

/*
 * ���ӻ����������صĴ�������
 */
static uint8_t DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;

    DS18B20_Mode_IPU(); /* ��������Ϊ�������� */
    while (DS18B20_DATA_IN() && pulse_time < 100)
    {
        pulse_time++;
        delay_us(1);
    }
    if (pulse_time >= 100) /* ����100us�󣬴������嶼��û�е���*/
        return 1;
    else
        pulse_time = 0;
    while (!DS18B20_DATA_IN() && pulse_time < 240) /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t dat;

    DS18B20_Mode_Out_PP(); /* ��0�Ͷ�1��ʱ������Ҫ����60us */
    DS18B20_DATA_OUT(LOW); /* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
    delay_us(10);
    DS18B20_Mode_IPU(); /* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
    //delay_us(2);
    if (DS18B20_DATA_IN() == SET)
        dat = 1;
    else
        dat = 0;
    delay_us(45); /* �����ʱ������ο�ʱ��ͼ */
    return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20_Read_Byte2(void)
{
    uint8_t i, j, dat = 0;

    for (i = 0; i < 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (dat) | (j << i);
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

    for (i = 0; i < 8; i++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        if (testb) /* д0��д1��ʱ������Ҫ����60us */
        {
            DS18B20_DATA_OUT(LOW);
            delay_us(8); /* 1us < �����ʱ < 15us */
            DS18B20_DATA_OUT(HIGH);
            delay_us(58);
        }
        else
        {
            DS18B20_DATA_OUT(LOW);
            delay_us(70); /* 60us < Tx 0 < 120us */
            DS18B20_DATA_OUT(HIGH);
            delay_us(2); /* 1us < Trec(�ָ�ʱ��) < �����*/
        }
    }
}

void DS18B20_Start(void)
{
    DS18B20_Rst2();
    DS18B20_Presence();
    DS18B20_Write_Byte2(0XCC); /* ���� ROM */
    DS18B20_Write_Byte2(0X44); /* ��ʼת�� */
}

uint8_t DS18B20_Init2(void)
{
    DS18B20_GPIO_Config();
    DS18B20_Rst2();
    return DS18B20_Presence();
}

float DS18B20_Get_Temp2(void)
{
    uint8_t tpmsb, tplsb;
    short s_tem;
    float f_tem;

    DS18B20_Rst2();
    DS18B20_Presence();
    DS18B20_Write_Byte2(0XCC); /* ���� ROM */
    DS18B20_Write_Byte2(0X44); /* ��ʼת�� */

    DS18B20_Rst2();
    DS18B20_Presence();
    DS18B20_Write_Byte2(0XCC); /* ���� ROM */
    DS18B20_Write_Byte2(0XBE); /* ���¶�ֵ */

    tplsb = DS18B20_Read_Byte2();
    tpmsb = DS18B20_Read_Byte2();

    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    f_tem = s_tem * 0.625;
    return f_tem;
}
