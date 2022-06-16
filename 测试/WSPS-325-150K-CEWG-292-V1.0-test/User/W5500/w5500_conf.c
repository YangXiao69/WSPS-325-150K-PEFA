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

#include "w5500_conf.h"
//#include "utility.h"
#include "w5500.h"

CONFIG_MSG  ConfigMsg;																	  /*配置结构体*/

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x21}; 
uint8 mac2[6]={0x00,0x08,0xdc,0x11,0x11,0x25};
/* 定义本机IP1信息*/
uint8 local_ip[4] ={192,168,1,100};										  /*定义设备接口W5500默认IP地址*/
uint8 subnet[4]={255,255,255,0};												  /*定义W5500默认子网掩码*/
uint8 gateway[4]={192,168,1,1};												  	/*定义W5500默认网关*/
uint16 local_port=5000;	                        				  /*定义本地端口*/

uint8  remote_ip[4]={255,255,255,255};							        /*远端IP地址*/
uint16 remote_port=5000;															 	    /*远端端口号*/



/* 定义IP2与上位机通信信息 */
uint8 local_ip2[4] ={192,168,1,105};										    /*定义上位机W5500默认IP地址*/
uint8 subnet2[4]   ={255,255,255,0};												/*定义W5500默认子网掩码*/
uint8 gateway2[4]  ={192,168,1,1};													/*定义W5500默认网关*/
uint16 local_port2 = 5000;	                        			  /*定义本地端口*/

uint8  remote_ip2[4]={192,168,1,99};							        /*上位机的IP*/
uint16 remote_port2=5000;															 	    /*远端端口号*/

/*****************************************************************************/
                            /* SPI4 相关 */
/*****************************************************************************/
/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void spi_w5500_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
    
    //设置引脚复用
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_SPI4); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_SPI4); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_SPI4); 
    
    /*!< 配置 SPI_FLASH_SPI 引脚: SCK */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /*!< 配置 SPI_FLASH_SPI 引脚: MISO */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_MISO_PIN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /*!< 配置 SPI_FLASH_SPI 引脚: MOSI */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_MOSI_PIN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
    /*!< 配置 SPI_FLASH_SPI 引脚: CS */
    GPIO_InitStructure.GPIO_Pin = W5500_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
    GPIOE->BSRRH=W5500_SPI_SCK_PIN;         /* 时钟拉低 */	
    GPIOE->BSRRL=GPIO_Pin_8;                /* 复位拉高 */	
    GPIOE->BSRRL=W5500_CS_PIN;              /* 停止信号 FLASH: CS引脚高电平*/
    
    /* FLASH_SPI 模式配置 */
    // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI4, &SPI_InitStructure);
    SPI_Cmd(SPI4, ENABLE);
}

/*
* 	W5500复位设置函数
*/
void reset_w5500(void)
{
    GPIOE->BSRRH=GPIO_Pin_8;
    delay_us(2);  
    GPIOE->BSRRL=GPIO_Pin_8;
    delay_ms(1600);
}

/*
*	 STM32 SPI1读写8位数据
*/
uint8 SPI4_SendByte(uint8 byte)
{
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);  /* 等待发送缓冲区为空，TXE事件 */
    SPI_I2S_SendData(SPI4, byte);                                    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET); /* 等待接收缓冲区非空，RXNE事件 */
    return SPI_I2S_ReceiveData(SPI4);                                /* 读取数据寄存器，获取接收缓冲区数据 */
}
/*
*  向固定地址写入一个8位数据到W5500
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
    W5500_CS_LOW();	
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);	
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8) + 4);  
    SPI4_SendByte(data);   
    W5500_CS_HIGH();	                     
}
/*
*		向固定地址从W5500读出一个8位数据
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
    uint8 data = 0;
    
    W5500_CS_LOW();	                       
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8))    ;
    data = SPI4_SendByte(0x00);            
    W5500_CS_HIGH();	 
                        
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
    
    W5500_CS_LOW();	                        
    idx = SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    idx = SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    idx = SPI4_SendByte( (addrbsb & 0x000000F8) + 4); 
    for(idx = 0; idx < len; idx++)
    {
    SPI4_SendByte(buf[idx]);
    }
    W5500_CS_HIGH();	
                        
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
    
    W5500_CS_LOW();	                        
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8));    
    for(idx = 0; idx < len; idx++)                   
    {
    buf[idx] = SPI4_SendByte(0x00);
    }
    W5500_CS_HIGH();	
                            
    return len;
}





/**********************************************************************************/
                              /*  SPI2 相关  */
/**********************************************************************************/
void spi_w5500_top_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    //设置引脚复用
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); 
    
    /*!< 配置 SPI_FLASH_SPI 引脚: SCK */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*!< 配置 SPI_FLASH_SPI 引脚: MISO */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_MISO_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*!< 配置 SPI_FLASH_SPI 引脚: MOSI */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_MOSI_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    /*!< 配置 SPI_FLASH_SPI 引脚: CS */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    GPIOB->BSRRH=W5500TOP_SPI_SCK_PIN;         /* 时钟拉低 */	
    GPIOB->BSRRL=GPIO_Pin_10;                  /* 复位拉高 */	
    GPIOB->BSRRL=W5500TOP_CS_PIN;              /* 停止信号 FLASH: CS引脚高电平*/
    
    
    /* FLASH_SPI 模式配置 */
    // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

/*
* 	W5500复位设置函数
*/
void reset_w5500_top(void)
{
    GPIOB->BSRRH=GPIO_Pin_10;                  /* 复位拉低 */	
		delay_us(2);  
    GPIOB->BSRRL=GPIO_Pin_10;                  /* 复位拉高 */	
		delay_ms(1600);
}

/*
*	 STM32 SPI1读写8位数据
*/
uint8 SPI2_SendByte(uint8 byte)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  /* 等待发送缓冲区为空，TXE事件 */
    SPI_I2S_SendData(SPI2, byte);                                    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); /* 等待接收缓冲区非空，RXNE事件 */
    return SPI_I2S_ReceiveData(SPI2);                                /* 读取数据寄存器，获取接收缓冲区数据 */
}                               //返回数据

/*
*  向固定地址写入一个8位数据到W5500
*/
void IINCHIP_WRITE_TOP( uint32 addrbsb,  uint8 data)
{
   W5500TOP_CS_LOW();	                   		
   SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);	
   SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
   SPI2_SendByte( (addrbsb & 0x000000F8) + 4);  
   SPI2_SendByte(data);                   
   W5500TOP_CS_HIGH();	            
}
/*
*		向固定地址从W5500读出一个8位数据
*/
uint8 IINCHIP_READ_TOP(uint32 addrbsb)
{
   uint8 data = 0;
   W5500TOP_CS_LOW();	
   SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
   SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
   SPI2_SendByte( (addrbsb & 0x000000F8))    ;
   data = SPI2_SendByte(0x00);            
   W5500TOP_CS_HIGH();	                         
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
   W5500TOP_CS_LOW();	
   idx = SPI2_SendByte((addrbsb & 0x00FF0000)>>16);
   idx = SPI2_SendByte((addrbsb & 0x0000FF00)>> 8);
   idx = SPI2_SendByte((addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     SPI2_SendByte(buf[idx]);
   }
   W5500TOP_CS_HIGH();	             
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

  W5500TOP_CS_LOW();	                  
  SPI2_SendByte( (addrbsb & 0x00FF0000)>>16);
  SPI2_SendByte( (addrbsb & 0x0000FF00)>> 8);
  SPI2_SendByte( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = SPI2_SendByte(0x00);
  }
  W5500TOP_CS_HIGH();	                             
  return len;
}


/**********************************************************************************/
void w5500_init(void)
{
    spi_w5500_config();	         	      /*初始化MCU相关引脚*/
    reset_w5500();					            /*硬复位W5500*/
        
    wiz_write_buf(SHAR0, mac, 6);       /*将IP配置信息写入W5500相应寄存器*/	             
    wiz_write_buf(SUBR0, subnet, 4);	
    wiz_write_buf(GAR0,  gateway, 4);
    wiz_write_buf(SIPR0, local_ip, 4);  
}


void w5500_top_init(void)
{
    spi_w5500_top_config();	         	      /*初始化MCU相关引脚*/
    reset_w5500_top();					            /*硬复位W5500*/
    
    wiz_write_buf_top(SHAR0, mac2, 6);          /*将IP配置信息写入W5500相应寄存器*/	             
    wiz_write_buf_top(SUBR0, subnet2, 4);	
 	wiz_write_buf_top(GAR0,  gateway2, 4);
 	wiz_write_buf_top(SIPR0, local_ip2, 4); 
}




