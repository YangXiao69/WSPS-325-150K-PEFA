/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "./../bsp/bsp.h"


#include "./w5500_conf.h"
#include "./w5500.h"


CONFIG_MSG  ConfigMsg;																	  /*���ýṹ��*/

uint8_t NumName = 0;

uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};           /*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 local_ip[4]={192,168,1, 101};									    /*���屾��IP��Ϣ*/	  

uint8 subnet[4]={255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway[4]={192,168,1,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/
uint16 local_port=8096;	                        				/*���屾�ض˿�*/


/*����Ŀ��IP��Ϣ*/
uint8  remote_ip[4]={192,168,1,100};							        /*Զ��IP��ַ*/
uint16 remote_port=8096;		 													  	/*Զ�˶˿ں�*/


void num_INIT(void)
{
    mac[5] = 0x10+NumName;
  	local_ip[3] = 100+NumName;
	

    if(NumName==17) mac[5]+=0x20;   //0x21  ���������ַ
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


/*****************************************************************************/
                            /* SPI2 ��� */
/*****************************************************************************/
/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void spi_w5500_config(void)
{
    SPI2_Init();
}

/*
* 	W5500��λ���ú���
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
*	 STM32 SPI1��д8λ����
*/
//uint8 SPI4_SendByte(uint8 byte)
//{
//    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
//    SPI_I2S_SendData(SPI4, byte);                                    /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
//    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET); /* �ȴ����ջ������ǿգ�RXNE�¼� */
//    return SPI_I2S_ReceiveData(SPI4);                                /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
//}


/*
*  ��̶���ַд��һ��8λ���ݵ�W5500
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
*		��̶���ַ��W5500����һ��8λ����
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
                              /*  SPI4 ���  */
/**********************************************************************************/
void spi_w5500_top_config(void)
{
//    InitSPI4();	
}

/*
* 	W5500��λ���ú���
*/
void reset_w5500_top(void)
{
//    GPIOB->BSRR=((uint32_t)GPIO_PIN_10 << 16U);                  /* ��λ���� */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_Delay(80);  
//    GPIOB->BSRR=GPIO_PIN_10;                  /* ��λ���� */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1600);
}

	
/*
*  ��̶���ַд��һ��8λ���ݵ�W5500
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
*		��̶���ַ��W5500����һ��8λ����
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
//    spi_w5500_config();	         	      /*��ʼ��MCU�������*/
//    reset_w5500();					            /*Ӳ��λW5500*/
//        
//    wiz_write_buf(SHAR0, mac, 6);       /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	             
//    wiz_write_buf(SUBR0, subnet, 4);	
//    wiz_write_buf(GAR0,  gateway, 4);
//    wiz_write_buf(SIPR0, local_ip, 4);  
//}


void w5500_init(void)
{
    num_INIT();
    spi_w5500_config();	         	      /*��ʼ��MCU�������*/
    reset_w5500();					            /*Ӳ��λW5500*/
    set_w5500_mac();                    /*���������ַ*/
    set_w5500_ip();                     /*����IP��ַ*/
}
