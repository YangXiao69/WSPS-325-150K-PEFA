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

#include "w5500_conf.h"
//#include "utility.h"
#include "w5500.h"

CONFIG_MSG  ConfigMsg;																	  /*���ýṹ��*/

/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x21}; 
uint8 mac2[6]={0x00,0x08,0xdc,0x11,0x11,0x25};
/* ���屾��IP1��Ϣ*/
uint8 local_ip[4] ={192,168,1,100};										  /*�����豸�ӿ�W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};												  /*����W5500Ĭ����������*/
uint8 gateway[4]={192,168,1,1};												  	/*����W5500Ĭ������*/
uint16 local_port=5000;	                        				  /*���屾�ض˿�*/

uint8  remote_ip[4]={255,255,255,255};							        /*Զ��IP��ַ*/
uint16 remote_port=5000;															 	    /*Զ�˶˿ں�*/



/* ����IP2����λ��ͨ����Ϣ */
uint8 local_ip2[4] ={192,168,1,105};										    /*������λ��W5500Ĭ��IP��ַ*/
uint8 subnet2[4]   ={255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway2[4]  ={192,168,1,1};													/*����W5500Ĭ������*/
uint16 local_port2 = 5000;	                        			  /*���屾�ض˿�*/

uint8  remote_ip2[4]={192,168,1,99};							        /*��λ����IP*/
uint16 remote_port2=5000;															 	    /*Զ�˶˿ں�*/

/*****************************************************************************/
                            /* SPI4 ��� */
/*****************************************************************************/
/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void spi_w5500_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
    
    //�������Ÿ���
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_SPI4); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_SPI4); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_SPI4); 
    
    /*!< ���� SPI_FLASH_SPI ����: SCK */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /*!< ���� SPI_FLASH_SPI ����: MISO */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_MISO_PIN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /*!< ���� SPI_FLASH_SPI ����: MOSI */
    GPIO_InitStructure.GPIO_Pin = W5500_SPI_MOSI_PIN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
    /*!< ���� SPI_FLASH_SPI ����: CS */
    GPIO_InitStructure.GPIO_Pin = W5500_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
    GPIOE->BSRRH=W5500_SPI_SCK_PIN;         /* ʱ������ */	
    GPIOE->BSRRL=GPIO_Pin_8;                /* ��λ���� */	
    GPIOE->BSRRL=W5500_CS_PIN;              /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
    
    /* FLASH_SPI ģʽ���� */
    // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
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
* 	W5500��λ���ú���
*/
void reset_w5500(void)
{
    GPIOE->BSRRH=GPIO_Pin_8;
    delay_us(2);  
    GPIOE->BSRRL=GPIO_Pin_8;
    delay_ms(1600);
}

/*
*	 STM32 SPI1��д8λ����
*/
uint8 SPI4_SendByte(uint8 byte)
{
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    SPI_I2S_SendData(SPI4, byte);                                    /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
    while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET); /* �ȴ����ջ������ǿգ�RXNE�¼� */
    return SPI_I2S_ReceiveData(SPI4);                                /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
}
/*
*  ��̶���ַд��һ��8λ���ݵ�W5500
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
*		��̶���ַ��W5500����һ��8λ����
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
                              /*  SPI2 ���  */
/**********************************************************************************/
void spi_w5500_top_config(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    //�������Ÿ���
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); 
    
    /*!< ���� SPI_FLASH_SPI ����: SCK */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*!< ���� SPI_FLASH_SPI ����: MISO */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_MISO_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*!< ���� SPI_FLASH_SPI ����: MOSI */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_SPI_MOSI_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    /*!< ���� SPI_FLASH_SPI ����: CS */
    GPIO_InitStructure.GPIO_Pin = W5500TOP_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    GPIOB->BSRRH=W5500TOP_SPI_SCK_PIN;         /* ʱ������ */	
    GPIOB->BSRRL=GPIO_Pin_10;                  /* ��λ���� */	
    GPIOB->BSRRL=W5500TOP_CS_PIN;              /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
    
    
    /* FLASH_SPI ģʽ���� */
    // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
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
* 	W5500��λ���ú���
*/
void reset_w5500_top(void)
{
    GPIOB->BSRRH=GPIO_Pin_10;                  /* ��λ���� */	
		delay_us(2);  
    GPIOB->BSRRL=GPIO_Pin_10;                  /* ��λ���� */	
		delay_ms(1600);
}

/*
*	 STM32 SPI1��д8λ����
*/
uint8 SPI2_SendByte(uint8 byte)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    SPI_I2S_SendData(SPI2, byte);                                    /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); /* �ȴ����ջ������ǿգ�RXNE�¼� */
    return SPI_I2S_ReceiveData(SPI2);                                /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
}                               //��������

/*
*  ��̶���ַд��һ��8λ���ݵ�W5500
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
*		��̶���ַ��W5500����һ��8λ����
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
    spi_w5500_config();	         	      /*��ʼ��MCU�������*/
    reset_w5500();					            /*Ӳ��λW5500*/
        
    wiz_write_buf(SHAR0, mac, 6);       /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	             
    wiz_write_buf(SUBR0, subnet, 4);	
    wiz_write_buf(GAR0,  gateway, 4);
    wiz_write_buf(SIPR0, local_ip, 4);  
}


void w5500_top_init(void)
{
    spi_w5500_top_config();	         	      /*��ʼ��MCU�������*/
    reset_w5500_top();					            /*Ӳ��λW5500*/
    
    wiz_write_buf_top(SHAR0, mac2, 6);          /*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	             
    wiz_write_buf_top(SUBR0, subnet2, 4);	
 	wiz_write_buf_top(GAR0,  gateway2, 4);
 	wiz_write_buf_top(SIPR0, local_ip2, 4); 
}




