#include "system.h"

u8 RF1;
u8 RF2;
u8 RF3;
u8 RF4;

u8 PowerFlag = 0;  /* 电源电压告警 */
u16 AlarmFlag = 0; /* 位定义：0-15  依次： 输入功率   输出功率  ........ 合成器温度 */
uint8 Rx_buff[32]; /*定义一个2KB的缓存*/
uint8 Tx_buff[40];
void do_udp(void);

void set_data(void)
{
    u16 CurrNum;
    CurrNum = Current[0] + Current[1] + Current[2] + Current[3];

    Tx_buff[0] = PowerFlag;
    Tx_buff[1] = AlarmFlag >> 8;
    Tx_buff[2] = AlarmFlag;
    Tx_buff[3] = VOLT_DEC >> 8;
    Tx_buff[4] = VOLT_DEC; //功放电压

    Tx_buff[5] = (u16)Pin >> 8;
    Tx_buff[6] = (u16)Pin; //输入功率
    Tx_buff[7] = (u16)Pout >> 8;
    Tx_buff[8] = (u16)Pout; //输出功率
    Tx_buff[9] = (u16)Pfout >> 8;
    Tx_buff[10] = (u16)Pfout; //反射功率

    Tx_buff[11] = Current[0] >> 8;
    Tx_buff[12] = Current[0]; //电流
    Tx_buff[13] = Current[1] >> 8;
    Tx_buff[14] = Current[1];
    Tx_buff[15] = Current[2] >> 8;
    Tx_buff[16] = Current[2];
    Tx_buff[17] = Current[3] >> 8;
    Tx_buff[18] = Current[3];

    Tx_buff[19] = temp[1] >> 8;
    Tx_buff[20] = temp[1]; //功放温度
    Tx_buff[21] = temp[3] >> 8;
    Tx_buff[22] = temp[3];
    Tx_buff[23] = temp[5] >> 8;
    Tx_buff[24] = temp[5];
    Tx_buff[25] = temp[7] >> 8;
    Tx_buff[26] = temp[7];

    Tx_buff[27] = temp[0] >> 8;
    Tx_buff[28] = temp[0]; //隔离器温度
    Tx_buff[29] = temp[2] >> 8;
    Tx_buff[30] = temp[2];
    Tx_buff[31] = temp[4] >> 8;
    Tx_buff[32] = temp[4];
    Tx_buff[33] = temp[6] >> 8;
    Tx_buff[34] = temp[6];
    Tx_buff[35] = temp[8] >> 8;
    Tx_buff[36] = temp[8]; //合成器温度

    Tx_buff[37] = CurrNum >> 8;
    Tx_buff[38] = CurrNum; //总电流
}

void error_serve(void)
{
    u8 i;
    static u8 VoltNum, PoutNum, PfoutNum;
    static u16 CurrNum[4] = {0, 0, 0, 0};
    static u16 TempNum[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (VOLT_DEC >= 520 || VOLT_DEC <= 440) //功放电压24到32V
    {
        VoltNum++;
        if (VoltNum >= 200)
        {
            //VoltNum=200;  PPT_SW(OFF); LED_OFF(RFOUT_LED);  PowerFlag=1;
        }
    }
    else
        VoltNum = 0;

    if (Pout > 2400)
    {
        PoutNum++;
        if (PoutNum >= 50)
        {
            PoutNum = 50;
            AlarmFlag |= 1 << 1;
        }
    }
    else
    {
        PoutNum = 0;
        AlarmFlag &= 0xFFFD;
    }

    if (Pfout > 2200)
    {
        PfoutNum++;
        if (PfoutNum >= 50)
        {
            PfoutNum = 50;
            LED_ON(VSWR_LED);
            AlarmFlag |= 1 << 2;
        }
    }
    else
    {
        PfoutNum = 0;
        LED_OFF(VSWR_LED);
        AlarmFlag &= 0xFFFB;
    }

    //		for(i=0;i<4;i++)
    //		{
    //				if(Current[i] > 180 ||())    CurrNum[i]++;
    //				else 				                 CurrNum[i] = 0;
    //
    //		}
    if (Current[0] > 230 || (Current[0] < 4 && RF1))
        CurrNum[0]++;
    else
        CurrNum[0] = 0;

    if (Current[1] > 230 || (Current[1] < 4 && RF2))
        CurrNum[1]++;
    else
        CurrNum[1] = 0;

    if (Current[2] > 230 || (Current[2] < 4 && RF3))
        CurrNum[2]++;
    else
        CurrNum[2] = 0;

    if (Current[3] > 230 || (Current[3] < 4 && RF4))
        CurrNum[3]++;
    else
        CurrNum[3] = 0;

    if (CurrNum[0] >= 900) /*累次告警次数从原来的20次增加到900次*/
    {
        CurrNum[0] = 900;
        LED_ON(CURR_LED);
        AlarmFlag |= 1 << 3;
    }
    else
    {
        AlarmFlag &= 0xFFF7;
    }

    if (CurrNum[1] >= 900)
    {
        CurrNum[1] = 900;
        LED_ON(CURR_LED);
        AlarmFlag |= 1 << 4;
    }
    else
    {
        AlarmFlag &= 0xFFEF;
    }

    if (CurrNum[2] >= 900)
    {
        CurrNum[2] = 900;
        LED_ON(CURR_LED);
        AlarmFlag |= 1 << 5;
    }
    else
    {
        AlarmFlag &= 0xFFDF;
    }

    if (CurrNum[3] >= 900)
    {
        CurrNum[3] = 900;
        LED_ON(CURR_LED);
        AlarmFlag |= 1 << 6;
    }
    else
    {
        AlarmFlag &= 0xFFBF;
    }

    if ((AlarmFlag & 0x0078) == 0)
        LED_OFF(CURR_LED);

    for (i = 0; i < 9; i++)
    {
        if (temp[i] > 700)
            TempNum[i]++;
        else
            TempNum[i] = 0;
    }
    if (TempNum[1] >= 3000)
    {
        TempNum[1] = 1000; //功放 1
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 7;
    }
    else
    {
        AlarmFlag &= 0xFF7F;
    }

    if (TempNum[3] >= 3000)
    {
        TempNum[3] = 1000; //功放 2
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 8;
    }
    else
    {
        AlarmFlag &= 0xFEFF;
    }

    if (TempNum[5] >= 3000)
    {
        TempNum[5] = 1000; //功放 3
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 9;
    }
    else
    {
        AlarmFlag &= 0xFDFF;
    }

    if (TempNum[7] >= 3000)
    {
        TempNum[7] = 1000; //功放 4
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 10;
    }
    else
    {
        AlarmFlag &= 0xFBFF;
    }

    if (TempNum[0] >= 3000)
    {
        TempNum[0] = 1000; //隔离器 1
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 11;
    }
    else
    {
        AlarmFlag &= 0xF7FF;
    }

    if (TempNum[2] >= 3000)
    {
        TempNum[2] = 1000; //隔离器 2
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 12;
    }
    else
    {
        AlarmFlag &= 0xEFFF;
    }

    if (TempNum[4] >= 3000)
    {
        TempNum[4] = 1000; //隔离器 3
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 13;
    }
    else
    {
        AlarmFlag &= 0xDFFF;
    }

    if (TempNum[6] >= 3000)
    {
        TempNum[6] = 1000; //隔离器 4
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 14;
    }
    else
    {
        AlarmFlag &= 0xBFFF;
    }

    if (TempNum[8] >= 3000)
    {
        TempNum[8] = 1000; //合成器
        LED_ON(TEMP_LED);
        AlarmFlag |= 1 << 15;
    }
    else
    {
        AlarmFlag &= 0x7FFF;
    }

    if ((AlarmFlag & 0xFF80) == 0)
        LED_OFF(TEMP_LED);
}

u8 addr_init(void)
{
    u8 addr = 0;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //  PC11 IO1 5     PC10 IO2 8     PA12 IO3 7
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8))
        addr += 128;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9))
        addr += 64;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10))
        addr += 32;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11))
        addr += 16;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12))
        addr += 8;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13))
        addr += 4;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14))
        addr += 2;
    if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15))
        addr += 1;
    if(addr == 0)
    {
        addr++;
    }
    return addr;
}

int main(void)
{
    u8 i;
    u16 j;
    systick_init(72); /*初始化Systick工作时钟*/

    USART_Configuration(); /* 串口配置 */
    DMA_Configuration();   /* DMA配置 */
    NVIC_Configuration();  /* 中断优先级设置 */

    ADC1_Init(); /* ADC配置 */
    AT24CXX_Init();
    DS18B20_Init();

    DS18B20_Init2();
    delay_ms(100);

    led_init();

    NumName = addr_init();
    w5500_init();                    /*初始化通信，包含插件地址*/
    socket_buf_init(txsize, rxsize); /*初始化8个Socket的发送接收缓存大小*/

    PPT_SW_init();
    LED_ON(POWER_LED); /* 电源指示灯上电亮 */

    K_value = AT24CXX_ReadLenByte(0, 2);
    if (K_value > 10000)
        K_value = 10000;
    K_value2 = AT24CXX_ReadLenByte(2, 2);
    if (K_value2 > 10000)
        K_value2 = 10000;
    K_value3 = AT24CXX_ReadLenByte(4, 2);
    if (K_value3 > 10000)
        K_value3 = 10000;
    
    
    while (1) /*循环执行的函数*/
    {
        if (ADC_CUR_OK)
        {
            for (i = 0; i < 8; i++)
            {
                delay_us(100);
                POWER_calculate();
            }
        }
        else
        {
            delay_us(240);
            CURR_calculate();
        }
        j++;
        do_udp(); /*UDP 数据回环测试*/
        if (j > 1200)
        {
            j = 0;
            DS18B20_Get_Temp();
            temp[8] = DS18B20_Get_Temp2();
        }
//        error_serve();

        RF1 = GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_0);
        RF2 = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9);
        RF3 = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8);
        RF4 = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5);
    }
}

void do_udp(void)
{
    uint16 len = 0;
    static u16 time = 0;
    static u16 time2 = 0;
    switch (getSn_SR(0)) /*获取socket的状态*/
    {
    case SOCK_CLOSED:                        /*socket处于关闭状态*/
        socket(0, Sn_MR_UDP, local_port, 0); /*初始化socket*/
        break;

    case SOCK_UDP: /* socket初始化完成*/
        if (getSn_IR(0) & Sn_IR_RECV)
            setSn_IR(0, Sn_IR_RECV); /*清接收中断*/

        time++;
        if (time >= 800) /* 250 * 2ms = 500ms */
        {
            time = 0;
            set_data();
            sendto(0, Tx_buff, 39, remote_ip, remote_port);
        }

        if ((len = getSn_RX_RSR(0)) > 0) /*接收到数据*/
        {
            recvfrom(0, Rx_buff, len, remote_ip, &remote_port); /*W5500接收计算机发送来的数据*/

            if (Rx_buff[0] != 0xFF)
            {
                if (Rx_buff[0] == 0x01)
                    LED_ON(RFOUT_LED);
                if (Rx_buff[0] == 0x00)
                    LED_OFF(RFOUT_LED);

                if (Rx_buff[NumName] & 0x08)
                    GPIO_SetBits(GPIOB, GPIO_Pin_5); //模块4
                else
                    GPIO_ResetBits(GPIOB, GPIO_Pin_5);

                if (Rx_buff[NumName] & 0x04)
                    GPIO_SetBits(GPIOB, GPIO_Pin_8); //模块3
                else
                    GPIO_ResetBits(GPIOB, GPIO_Pin_8);

                if (Rx_buff[NumName] & 0x02)
                    GPIO_SetBits(GPIOB, GPIO_Pin_9); //模块2
                else
                    GPIO_ResetBits(GPIOB, GPIO_Pin_9);

                if (Rx_buff[NumName] & 0x01)
                    GPIO_SetBits(GPIOE, GPIO_Pin_0); //模块1
                else
                    GPIO_ResetBits(GPIOE, GPIO_Pin_0);
            }
            else
            {
                if (Rx_buff[1] == NumName)
                {
                    if (Rx_buff[2] == 1) //校准输出功率
                    {
                        K_value = ((u16)Rx_buff[3] << 8) + Rx_buff[4];
                        AT24CXX_WriteLenByte(0, K_value, 2);
                    }
                    else if (Rx_buff[2] == 2) //校准输入功率
                    {
                        K_value2 = ((u16)Rx_buff[3] << 8) + Rx_buff[4];
                        AT24CXX_WriteLenByte(2, K_value2, 2);
                    }
                    else if (Rx_buff[2] == 3) //校准反射功率
                    {
                        K_value3 = ((u16)Rx_buff[3] << 8) + Rx_buff[4];
                        AT24CXX_WriteLenByte(4, K_value3, 2);
                    }
                }
            }
            LED_ON(COMM_LED);
        }
        if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3))
            time2++;
        else
            time2 = 0;
        if (time2 >= 200)
        {
            time2 = 0;
            LED_OFF(COMM_LED);
        }
        break;
    }
}
