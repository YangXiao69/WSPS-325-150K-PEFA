/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "./../bsp/bsp.h"


#include "./w5500_conf.h"
#include "./w5500.h"


CONFIG_MSG  ConfigMsg;																	  /*配置结构体*/

uint8_t NumName = 0;

uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};           /*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 local_ip[4]={192,168,1, 101};									    /*定义本机IP信息*/	  

uint8 subnet[4]={255,255,255,0};												/*定义W5500默认子网掩码*/
uint8 gateway[4]={192,168,1,1};													/*定义W5500默认网关*/
uint8 dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/
uint16 local_port=8096;	                        				/*定义本地端口*/


/*定义目标IP信息*/
uint8  remote_ip[4]={192,168,1,100};							        /*远端IP地址*/
uint16 remote_port=8096;		 													  	/*远端端口号*/


void num_INIT(void)
{
    mac[5] = 0x10+NumName;
  	local_ip[3] = 100+NumName;
	

    if(NumName==17) mac[5]+=0x20;   //0x21  主控物理地址
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


/*****************************************************************************/
                            /* SPI2 相关 */
/*****************************************************************************/
/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void spi_w5500_config(void)
{
    SPI2_Init();
}

/*
* 	W5500复位设置函数
*/
void reset_w5500(void)
{
//    GPIOE->BSRR = ((uint32_t)GPIO_PIN_8 << 16U);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	
    HAL_Delay(50);  
//    GPIOE->BSRR = (GPIO_PIN_8 & 0xFFFF);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
    HAL_Delay(100);
}

/*
*	 STM32 SPI1读写8位数据
*/
//uint8 SPI4_SendByte(uint8 byte)
//{
//    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);  /* 等待发送缓冲区为空，TXE事件 */
//    SPI_I2S_SendData(SPI4, byte);                                    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
//    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET); /* 等待接收缓冲区非空，RXNE事件 */
//    return SPI_I2S_ReceiveData(SPI4);                                /* 读取数据寄存器，获取接收缓冲区数据 */
//}


/*
*  向固定地址写入一个8位数据到W5500
	}
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
//    W5500_SPIx_CS_LOW();	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
    SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);	
    SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI2_SendByte( (addrbsb & 0x000000F8) + 4);  
    SPI2_SendByte(data); 
//	W5500_SPIx_CS_HIGH();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	
}
/*
*		向固定地址从W5500读出一个8位数据
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
    uint8 data = 0;
    
//    W5500_SPIx_CS_LOW();	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
    SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI2_SendByte( (addrbsb & 0x000000F8))    ;
    data = SPI2_SendByte(0x00);            
// 	W5500_SPIx_CS_HIGH();	 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                        
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
    
//    W5500_SPIx_CS_LOW();
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	
    idx = SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
    idx = SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
    idx = SPI2_SendByte( (addrbsb & 0x000000F8) + 4); 
    for(idx = 0; idx < len; idx++)
    {
		SPI2_SendByte(buf[idx]);
    }
//	W5500_SPIx_CS_HIGH();	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                        
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
    
//    W5500_SPIx_CS_LOW();	 
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);	
    SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI2_SendByte( (addrbsb & 0x000000F8));    
    for(idx = 0; idx < len; idx++)                   
    {
		buf[idx] = SPI2_SendByte(0x00);
    }
//	W5500_SPIx_CS_HIGH();	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                            
    return len;
}





/**********************************************************************************/
                              /*  SPI4 相关  */
/**********************************************************************************/
void spi_w5500_top_config(void)
{
//    InitSPI4();	
}

/*
* 	W5500复位设置函数
*/
void reset_w5500_top(void)
{
//    GPIOB->BSRR=((uint32_t)GPIO_PIN_10 << 16U);                  /* 复位拉低 */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(80);  
//    GPIOB->BSRR=GPIO_PIN_10;                  /* 复位拉高 */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1600);
}

	
/*
*  向固定地址写入一个8位数据到W5500
*/
void IINCHIP_WRITE_TOP( uint32 addrbsb,  uint8 data)
{
//   W5500_SPIx_TOP_CS_LOW();	
   HAL_GPIO_WritePin (W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_RESET);	
   SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);	
   SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
   SPI2_SendByte( (addrbsb & 0x000000F8) + 4);  
   SPI2_SendByte(data);                   
//   W5500_SPIx_TOP_CS_HIGH();	
   HAL_GPIO_WritePin (W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_SET);	
}
/*
*		向固定地址从W5500读出一个8位数据
*/
uint8 IINCHIP_READ_TOP(uint32 addrbsb)
{
   uint8 data = 0;
//   W5500_SPIx_TOP_CS_LOW();	
   HAL_GPIO_WritePin(W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_RESET); 
   SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
   SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
   SPI2_SendByte( (addrbsb & 0x000000F8))    ;
   data = SPI2_SendByte(0x00);            
//   W5500_SPIx_TOP_CS_HIGH();
   HAL_GPIO_WritePin(W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_SET);	
   return data;     
}
/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf_top(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
//   W5500_SPIx_TOP_CS_LOW();	
   HAL_GPIO_WritePin(W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_RESET);	
   idx = SPI2_SendByte((addrbsb & 0x00FF0000)>>16);
   idx = SPI2_SendByte((addrbsb & 0x0000FF00)>> 8);
   idx = SPI2_SendByte((addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     SPI2_SendByte(buf[idx]);
   }
//   W5500_SPIx_TOP_CS_HIGH();	
   HAL_GPIO_WritePin(W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_SET);   
   return len;  
}



/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf_top(uint32 addrbsb, uint8* buf,uint16 len)
{
	uint16 idx = 0;

//  W5500_SPIx_TOP_CS_LOW();	  
	HAL_GPIO_WritePin (W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_RESET);	
	SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
	SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
	SPI2_SendByte( (addrbsb & 0x000000F8));    
	for(idx = 0; idx < len; idx++)                   
	{
		buf[idx] = SPI2_SendByte(0x00);
	}
//  W5500_SPIx_TOP_CS_HIGH();	    
	HAL_GPIO_WritePin (W5500_TOP_SPIx_CS_GPIO,W5500_TOP_SPIx_CS_PIN,GPIO_PIN_SET);  
	return len;
}











/**********************************************************************************/
//void w5500_init(void)
//{
//    spi_w5500_config();	         	      /*初始化MCU相关引脚*/
//    reset_w5500();					            /*硬复位W5500*/
//        
//    wiz_write_buf(SHAR0, mac, 6);       /*将IP配置信息写入W5500相应寄存器*/	             
//    wiz_write_buf(SUBR0, subnet, 4);	
//    wiz_write_buf(GAR0,  gateway, 4);
//    wiz_write_buf(SIPR0, local_ip, 4);  
//}


void w5500_init(void)
{
    num_INIT();
    spi_w5500_config();	         	      /*初始化MCU相关引脚*/
    reset_w5500();					            /*硬复位W5500*/
    set_w5500_mac();                    /*配置物理地址*/
    set_w5500_ip();                     /*配置IP地址*/
}
