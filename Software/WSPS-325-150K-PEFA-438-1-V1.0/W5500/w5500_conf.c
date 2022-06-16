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
#include "../../App/task.h"
#include "./w5500_conf.h"
#include "spi.h"


CONFIG_MSG  ConfigMsg;																	  /*���ýṹ��*/

/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8_t mac[6]={0x00,0x08,0xdc,0x11,0x11,0x21}; 
uint8_t mac2[6]={0x00,0x08,0xdc,0x11,0x11,0x25};
/* ���屾��IP1��Ϣ*/
uint8_t local_ip[4] ={192,168,1,100};										  /*�����豸�ӿ�W5500Ĭ��IP��ַ*/
uint8_t subnet[4]={255,255,255,0};												  /*����W5500Ĭ����������*/
uint8_t gateway[4]={192,168,1,1};												  	/*����W5500Ĭ������*/
uint16_t local_port=5000;	                        				  /*���屾�ض˿�*/

uint8_t  remote_ip[4]={255,255,255,255};							        /*Զ��IP��ַ*/
uint16_t remote_port=5000;															 	    /*Զ�˶˿ں�*/

/* ����IP2����λ��ͨ����Ϣ */
uint8_t local_ip2[4] ={192,168,1,105};										    /*������λ��W5500Ĭ��IP��ַ*/
uint8_t subnet2[4]   ={255,255,255,0};												/*����W5500Ĭ����������*/
uint8_t gateway2[4]  ={192,168,1,1};													/*����W5500Ĭ������*/
uint16_t local_port2 = 5000;	                        			  /*���屾�ض˿�*/

uint8_t  remote_ip2[4]={192,168,1,99};							        /*��λ����IP*/
uint16_t remote_port2=5000;															 	 /*Զ�˶˿ں�*/



uint8_t SPI4_SendByte(uint8_t byte)
{
	uint8_t SPI4TxBuf = 0;
	uint8_t SPI4RxBuf = 0;

	uint8_t retval = 0;

	SPI4TxBuf = byte;

	if (HAL_SPI_TransmitReceive(&hspi4, &SPI4TxBuf, &SPI4RxBuf, 1, 1000000) != HAL_OK)
	{
//		Error_Handler(__FILE__, __LINE__);
	}
	retval = SPI4RxBuf;

	return retval;
}


/*
* 	W5500��λ���ú���
*/
void reset_w5500(void)
{
//    GPIOE->BSRR = ((uint32_t)GPIO_PIN_8 << 16U);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);	
    delay_ms(50);  
//    GPIOE->BSRR = (GPIO_PIN_8 & 0xFFFF);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
    delay_ms(100);
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
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);	
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8) + 4);  
    SPI4_SendByte(data); 
//	W5500_SPIx_CS_HIGH();
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);	
}
/*
*		��̶���ַ��W5500����һ��8λ����
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
    uint8 data = 0;
    
//    W5500_SPIx_CS_LOW();	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8))    ;
    data = SPI4_SendByte(0x00);            
// 	W5500_SPIx_CS_HIGH();	 
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);                        
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
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	
    idx = SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    idx = SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    idx = SPI4_SendByte( (addrbsb & 0x000000F8) + 4); 
    for(idx = 0; idx < len; idx++)
    {
		SPI4_SendByte(buf[idx]);
    }
//	W5500_SPIx_CS_HIGH();	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);                        
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
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);	
    SPI4_SendByte( (addrbsb & 0x00FF0000)>>16);
    SPI4_SendByte( (addrbsb & 0x0000FF00)>> 8);
    SPI4_SendByte( (addrbsb & 0x000000F8));    
    for(idx = 0; idx < len; idx++)                   
    {
		buf[idx] = SPI4_SendByte(0x00);
    }
//	W5500_SPIx_CS_HIGH();	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);                            
    return len;
}


/*******************************************************************************/




/*
 *	 STM32 SPI2��д8λ����
 */
uint8_t SPI2_SendByte(uint8_t byte)
{
	uint8_t status = 0;
	uint8_t SPI2TxBuf = 0;
	uint8_t SPI2RxBuf = 0;

	uint8_t retval = 0;

	SPI2TxBuf = byte;

	status = HAL_SPI_TransmitReceive(&hspi2, &SPI2TxBuf, &SPI2RxBuf, 1, 1000000);

	if (status != HAL_OK)
	{
//		Error_Handler(__FILE__, __LINE__);
	}

	retval = SPI2RxBuf;

	return retval;
}


/*
* 	W5500��λ���ú���
*/
void reset_w5500_top(void)
{
//    GPIOB->BSRR=((uint32_t)GPIO_PIN_10 << 16U);                  /* ��λ���� */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	delay_ms(80);  
//    GPIOB->BSRR=GPIO_PIN_10;                  /* ��λ���� */	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
	delay_ms(1600);
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
void w5500_init(void)
{
    reset_w5500();					            /*Ӳ��λW5500*/
        
    wiz_write_buf(SHAR0, mac, 6);       /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	             
    wiz_write_buf(SUBR0, subnet, 4);	
    wiz_write_buf(GAR0,  gateway, 4);
    wiz_write_buf(SIPR0, local_ip, 4);  
}


void w5500_top_init(void)
{
    reset_w5500_top();					            /*Ӳ��λW5500*/
    
    wiz_write_buf_top(SHAR0, mac2, 6);          /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	             
    wiz_write_buf_top(SUBR0, subnet2, 4);	
 	wiz_write_buf_top(GAR0,  gateway2, 4);
 	wiz_write_buf_top(SIPR0, local_ip2, 4); 
}




