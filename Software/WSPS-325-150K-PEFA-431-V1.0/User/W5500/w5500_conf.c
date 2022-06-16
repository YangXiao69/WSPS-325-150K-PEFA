/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
**************************************************************************************************
*/
#include "system.h"

CONFIG_MSG  ConfigMsg;																	  /*配置结构体*/



uint8 mac[6]= {0x00,0x08,0xdc,0x11,0x11,0x11}; /*物理地址*/
uint8 local_ip[4]= {192,168,1, 101};					 /*IP地址*/



uint8 subnet[4]= {255,255,255,0};												/*定义W5500默认子网掩码*/
uint8 gateway[4]= {192,168,1,1};													/*定义W5500默认网关*/
uint8 dns_server[4]= {114,114,114,114};									/*定义W5500默认DNS*/
uint16 local_port=5000;	                        				/*定义本地端口*/


/*定义目标IP信息*/
uint8  remote_ip[4]= {192,168,1,100};							       /*远端IP地址*/
uint16 remote_port=5000;		 													  	/*远端端口号*/


void Set_IP(void)
{
    switch(System.Addr)                                                 /*获取socket的状态*/
    {
    case 1:
        mac[5]= 0x11;
        local_ip[3]=101;// K_value=440; K_value2=450;
        break;

    case 2:
        mac[5]= 0x12;
        local_ip[3]=102;// K_value=470; K_value2=2300;
        break;

    case 3:
        mac[5]= 0x13;
        local_ip[3]=103;// K_value=440; K_value2=500;
        break;

    case 4:
        mac[5]= 0x14;
        local_ip[3]=104;// K_value=450; K_value2=450;
        break;

    case 5:
        mac[5]= 0x15;
        local_ip[3]=105;// K_value=450; K_value2=450;
        break;
    
    case 6:
        mac[5]= 0x16;
        local_ip[3]=106;// K_value=450; K_value2=450;
        break;
    
    case 7:
        mac[5]= 0x17;
        local_ip[3]=107;// K_value=450; K_value2=450;
        break;    

    case 8:
        mac[5]= 0x18;
        local_ip[3]=108;// K_value=450; K_value2=450;
        break;

    case 9:
        mac[5]= 0x19;
        local_ip[3]=109;// K_value=450; K_value2=450;
        break;

    case 10:
        mac[5]= 0x1a;
        local_ip[3]=110;// K_value=450; K_value2=450;
        break;

    case 11:
        mac[5]= 0x1b;
        local_ip[3]=111;// K_value=450; K_value2=450;
        break;

    case 12:
        mac[5]= 0x1c;
        local_ip[3]=112;// K_value=450; K_value2=450;
        break;

    case 13:
        mac[5]= 0x1d;
        local_ip[3]=113;// K_value=450; K_value2=450;
        break;

    case 14:
        mac[5]= 0x1e;
        local_ip[3]=114;// K_value=450; K_value2=450;
        break;

    case 15:
        mac[5]= 0x1f;
        local_ip[3]=115;// K_value=450; K_value2=450;
        break;    

    case 16:
        mac[5]= 0x20;
        local_ip[3]=116;// K_value=450; K_value2=450;
        break;
    
    default:
        break;
    }
}

/**
*@brief		配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{
    /*复制定义的配置信息到配置结构体*/
    memcpy(ConfigMsg.mac, mac, 6);
    memcpy(ConfigMsg.lip,local_ip,4);
    memcpy(ConfigMsg.sub,subnet,4);
    memcpy(ConfigMsg.gw,gateway,4);
    memcpy(ConfigMsg.dns,dns_server,4);

    /*将IP配置信息写入W5500相应寄存器*/
    setSUBR(ConfigMsg.sub);
    setGAR(ConfigMsg.gw);
    setSIPR(ConfigMsg.lip);

    getSIPR (local_ip);	 /* IP地址 */
    getSUBR(subnet);     /* 子网掩码 */
    getGAR(gateway);     /* 网关 */

}

/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
    memcpy(ConfigMsg.mac, mac, 6);
    setSHAR(ConfigMsg.mac);	/**/
}

/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void spi_w5500_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* 初始化SCK、MISO、MOSI引脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 初始化CS引脚 */
    GPIO_InitStructure.GPIO_Pin = WIZ_SCS;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*定义RESET引脚*/
    GPIO_InitStructure.GPIO_Pin = WIZ_RESET;					   /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*设置引脚模式为通用推挽输出*/
    GPIO_Init(GPIOD, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
    GPIO_SetBits(GPIOD, WIZ_RESET);

    /* 初始化配置STM32 SPI2 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    /* 全双工 */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         /* 主机 */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     /* 8位帧结构 */
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           /* 时钟悬空高 */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          /* 数据采样开始时钟 */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             /*  */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;    /* 波特率分频值2，即时钟频率的1/2 */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2,ENABLE);	  //STM32使能SPI1
}

uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//等待数据寄存器空
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, byte);
    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//等待数据寄存器空
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
    GPIO_ResetBits(GPIOD, WIZ_RESET);
    delay_ms(2);
    GPIO_SetBits(GPIOD, WIZ_RESET);
    delay_ms(1600);
}


/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
    return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
    GPIO_ResetBits(GPIOB, WIZ_SCS);
    IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
    IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
    IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);
    IINCHIP_SpiSendData(data);
    GPIO_SetBits(GPIOB, WIZ_SCS);
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
    uint8 data = 0;
    GPIO_ResetBits(GPIOB, WIZ_SCS);
    IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
    IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
    IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
    data = IINCHIP_SpiSendData(0x00);
    GPIO_SetBits(GPIOB, WIZ_SCS);
    return data;
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
    uint16 idx = 0;
    GPIO_ResetBits(GPIOB, WIZ_SCS);
    idx = IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
    idx = IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
    idx = IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);
    for(idx = 0; idx < len; idx++)
    {
        IINCHIP_SpiSendData(buf[idx]);
    }
    GPIO_SetBits(GPIOB, WIZ_SCS);
    return len;
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
    uint16 idx = 0;

    GPIO_ResetBits(GPIOB, WIZ_SCS);
    IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
    IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
    IINCHIP_SpiSendData( (addrbsb & 0x000000F8));
    for(idx = 0; idx < len; idx++)
    {
        buf[idx] = IINCHIP_SpiSendData(0x00);
    }
    GPIO_SetBits(GPIOB, WIZ_SCS);
    return len;
}


/**
*@brief		STM32系统软复位函数
*@param		无
*@return	无
*/
void reboot(void)
{
    pFunction Jump_To_Application;
    uint32 JumpAddress;

    JumpAddress = *(vu32*) (0x00000004);
    Jump_To_Application = (pFunction) JumpAddress;
    Jump_To_Application();
}


void w5500_init(void)
{
    Set_IP();
    spi_w5500_config();	         	      /*初始化MCU相关引脚*/
    reset_w5500();					            /*硬复位W5500*/
    set_w5500_mac();                    /*配置物理地址*/
    set_w5500_ip();                     /*配置IP地址*/
}

