/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include "system.h"

CONFIG_MSG  ConfigMsg;																	  /*���ýṹ��*/



uint8 mac[6]= {0x00,0x08,0xdc,0x11,0x11,0x11}; /*�����ַ*/
uint8 local_ip[4]= {192,168,1, 101};					 /*IP��ַ*/



uint8 subnet[4]= {255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway[4]= {192,168,1,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]= {114,114,114,114};									/*����W5500Ĭ��DNS*/
uint16 local_port=5000;	                        				/*���屾�ض˿�*/


/*����Ŀ��IP��Ϣ*/
uint8  remote_ip[4]= {192,168,1,100};							       /*Զ��IP��ַ*/
uint16 remote_port=5000;		 													  	/*Զ�˶˿ں�*/


void Set_IP(void)
{
    switch(System.Addr)                                                 /*��ȡsocket��״̬*/
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
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{
    /*���ƶ����������Ϣ�����ýṹ��*/
    memcpy(ConfigMsg.mac, mac, 6);
    memcpy(ConfigMsg.lip,local_ip,4);
    memcpy(ConfigMsg.sub,subnet,4);
    memcpy(ConfigMsg.gw,gateway,4);
    memcpy(ConfigMsg.dns,dns_server,4);

    /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/
    setSUBR(ConfigMsg.sub);
    setGAR(ConfigMsg.gw);
    setSIPR(ConfigMsg.lip);

    getSIPR (local_ip);	 /* IP��ַ */
    getSUBR(subnet);     /* �������� */
    getGAR(gateway);     /* ���� */

}

/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
    memcpy(ConfigMsg.mac, mac, 6);
    setSHAR(ConfigMsg.mac);	/**/
}

/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void spi_w5500_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* ��ʼ��SCK��MISO��MOSI���� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ��ʼ��CS���� */
    GPIO_InitStructure.GPIO_Pin = WIZ_SCS;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*����RESET����*/
    GPIO_InitStructure.GPIO_Pin = WIZ_RESET;					   /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*��������ģʽΪͨ���������*/
    GPIO_Init(GPIOD, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
    GPIO_SetBits(GPIOD, WIZ_RESET);

    /* ��ʼ������STM32 SPI2 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    /* ȫ˫�� */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         /* ���� */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     /* 8λ֡�ṹ */
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           /* ʱ�����ո� */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          /* ���ݲ�����ʼʱ�� */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             /*  */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;    /* �����ʷ�Ƶֵ2����ʱ��Ƶ�ʵ�1/2 */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2,ENABLE);	  //STM32ʹ��SPI1
}

uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ݼĴ�����
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, byte);
    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ����ݼĴ�����
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void reset_w5500(void)
{
    GPIO_ResetBits(GPIOD, WIZ_RESET);
    delay_ms(2);
    GPIO_SetBits(GPIOD, WIZ_RESET);
    delay_ms(1600);
}


/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
    return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
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
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
*@brief		STM32ϵͳ��λ����
*@param		��
*@return	��
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
    spi_w5500_config();	         	      /*��ʼ��MCU�������*/
    reset_w5500();					            /*Ӳ��λW5500*/
    set_w5500_mac();                    /*���������ַ*/
    set_w5500_ip();                     /*����IP��ַ*/
}

