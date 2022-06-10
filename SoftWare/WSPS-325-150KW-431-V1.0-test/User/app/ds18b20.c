/*
* 10路 温度数据管脚对应
*	GPIO_D0-7  GPIO_D15
*/
#include "ds18b20.h"
#include "utility.h"

#define DS18B20_DQ_OUT0 GPIO_ResetBits(GPIOD, 0X00FF) //所有数据脚输出 0
#define DS18B20_DQ_OUT1 GPIO_SetBits(GPIOD, 0X00FF)   //所有数据脚输出 1

#define AP_temp1 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) //读取输入
#define AP_temp2 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) //读取输入
#define AP_temp3 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) //读取输入
#define AP_temp4 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) //读取输入
#define AP_temp5 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) //读取输入
#define AP_temp6 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) //读取输入
#define AP_temp7 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) //读取输入
#define AP_temp8 GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) //读取输入

int16_t temp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; /* 隔离器1-4   temp[0] temp[2] temp[4] temp[6]:
                                         功放  1-4   temp[1] temp[3] temp[5] temp[7]:
                                         合成器      temp[8]  */

void DS18B20_IO_OUT(void) /* 数据脚输出模式 */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = 0X00FF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* 50M时钟速度 */
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void DS18B20_IO_IN(void) /* 数据脚输入模式 */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = 0X00FF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/*
*  对应端口读取
*  输出的16位分别对应输入位
*/
u16 DQ_IN(void) /* 数据脚输入读取 */
{
    u16 date;
    date = GPIO_ReadInputData(GPIOD) & 0X00FF;
    return date;
}

/* 复位DS18B20 */
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();
    GPIO_ResetBits(GPIOD, 0X00FF); //所有数据脚输出 0
    delay_us(750);                 //拉低750us
    GPIO_SetBits(GPIOD, 0X00FF);   //DQ=1
    delay_us(1);
}

/* 等待DS18B20的回应，返回值为0X00FF 则全部正常，异常则返回异常值 */
u16 DS18B20_Check(void)
{
    u8 retry = 0;

    DS18B20_IO_IN();
    while (DQ_IN() && retry < 200) /* 等待脉冲低电平到来（等待200us）*/
    {
        retry++;
        delay_us(1);
    };
    if (retry >= 200)
        return 1;
    else
        retry = 0;
    while ((DQ_IN() != 0X00FF) && retry < 240) /* 10个中时间最长的要小于240us（低电平持续的时间） */
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

/* 初始化DS18B20的IO口， DQ 同时检测DS的存在：返回1:不存在。返回0:存在 */
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

/* 写一个字节到DS18B20，十组同时写入同样的数据 */
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

/* 向18B20读取一个字节 */
uint8_t DS18B20_Read_Byte(u8 num)
{
    u8 i, j;
    uint8_t dat = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* 50M时钟速度 */

    switch (num)
    {
    case 0:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_7);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp1;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 1:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_6);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp2;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 2:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_5);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp3;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 3:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_4);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp4;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 4:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_3);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp5;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 5:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_2);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp6;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 6:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_1);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp7;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
    case 7:
        for (i = 0; i < 8; i++)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; /* 输出模式 */
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            GPIO_ResetBits(GPIOD, GPIO_Pin_0);
            delay_us(2);
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 上拉输入 */
            GPIO_Init(GPIOD, &GPIO_InitStructure);
            j = AP_temp8;
            dat = (dat) | (j << i);
            delay_us(45);
        }
        break;
        //		case 8: for(i=0; i<8; i++)
        //            {
        //								GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;            /* 输出模式 */
        //								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        //								GPIO_Init(GPIOD, &GPIO_InitStructure);
        //								GPIO_ResetBits(GPIOD,GPIO_Pin_15);
        //								delay_us(2);
        //								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        /* 上拉输入 */
        //								GPIO_Init(GPIOD, &GPIO_InitStructure);
        //							  j = AP_temp8;
        //							  dat = (dat) | (j<<i);
        //						    delay_us(45);
        //						}	  break;
    }
    return dat;
}

/* 从ds18b20得到10组温度值，得到的温度值存放于数组 temp[10] 中 */
void DS18B20_Get_Temp(void)
{
    u8 i;
    uint8_t tpmsb, tplsb;
    short s_tem;

    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte(0X44); /* 开始转换 */

    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte(0XBE); /* 读温度值 */

    for (i = 0; i < 8; i++)
    {
        tplsb = DS18B20_Read_Byte(i);
        tpmsb = DS18B20_Read_Byte(i);
        s_tem = tpmsb << 8;
        s_tem = s_tem | tplsb;
        if (s_tem < 0)                      /* 负温度 */
            temp[i] = (~s_tem + 1) * 0.625; /* 得到的温度值为整数，即比实际值放大10倍 */
        else
            temp[i] = s_tem * 0.625;
    }
}

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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：DS18B20_Mode_Out_PP
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
 *主机给从机发送复位脉冲
 */
static void DS18B20_Rst2(void)
{
    DS18B20_Mode_Out_PP(); /* 主机设置为推挽输出 */
    DS18B20_DATA_OUT(LOW);
    delay_us(750);          /* 主机至少产生480us的低电平复位信号 */
    DS18B20_DATA_OUT(HIGH); /* 主机在产生复位信号后，需将总线拉高 */
    delay_us(15);           /*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
}

/*
 * 检测从机给主机返回的存在脉冲
 */
static uint8_t DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;

    DS18B20_Mode_IPU(); /* 主机设置为上拉输入 */
    while (DS18B20_DATA_IN() && pulse_time < 100)
    {
        pulse_time++;
        delay_us(1);
    }
    if (pulse_time >= 100) /* 经过100us后，存在脉冲都还没有到来*/
        return 1;
    else
        pulse_time = 0;
    while (!DS18B20_DATA_IN() && pulse_time < 240) /* 存在脉冲到来，且存在的时间不能超过240us */
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
static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t dat;

    DS18B20_Mode_Out_PP(); /* 读0和读1的时间至少要大于60us */
    DS18B20_DATA_OUT(LOW); /* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
    delay_us(10);
    DS18B20_Mode_IPU(); /* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
    //delay_us(2);
    if (DS18B20_DATA_IN() == SET)
        dat = 1;
    else
        dat = 0;
    delay_us(45); /* 这个延时参数请参考时序图 */
    return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
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
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte2(uint8_t dat)
{
    uint8_t i, testb;
    DS18B20_Mode_Out_PP();

    for (i = 0; i < 8; i++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        if (testb) /* 写0和写1的时间至少要大于60us */
        {
            DS18B20_DATA_OUT(LOW);
            delay_us(8); /* 1us < 这个延时 < 15us */
            DS18B20_DATA_OUT(HIGH);
            delay_us(58);
        }
        else
        {
            DS18B20_DATA_OUT(LOW);
            delay_us(70); /* 60us < Tx 0 < 120us */
            DS18B20_DATA_OUT(HIGH);
            delay_us(2); /* 1us < Trec(恢复时间) < 无穷大*/
        }
    }
}

void DS18B20_Start(void)
{
    DS18B20_Rst2();
    DS18B20_Presence();
    DS18B20_Write_Byte2(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte2(0X44); /* 开始转换 */
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
    DS18B20_Write_Byte2(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte2(0X44); /* 开始转换 */

    DS18B20_Rst2();
    DS18B20_Presence();
    DS18B20_Write_Byte2(0XCC); /* 跳过 ROM */
    DS18B20_Write_Byte2(0XBE); /* 读温度值 */

    tplsb = DS18B20_Read_Byte2();
    tpmsb = DS18B20_Read_Byte2();

    s_tem = tpmsb << 8;
    s_tem = s_tem | tplsb;

    f_tem = s_tem * 0.625;
    return f_tem;
}
