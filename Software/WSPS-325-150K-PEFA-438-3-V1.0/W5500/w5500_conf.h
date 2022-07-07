#ifndef __W5500_CONF_H
#define __W5500_CONF_H

#include "stdio.h"
#include "types.h"
#include "main.h"
#include "./w5500.h"
#include "./socket.h"

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															/*Զ��IP��ַ*/
extern uint8    remote_ip2[4];
extern uint16 	remote_port;															/*Զ�˶˿ں�*/
extern uint16   remote_port2;
extern uint16   local_port;																/*���屾�ض˿�*/

extern uint8    local_ip[4];										          /*������λ��W5500Ĭ��IP��ַ*/
extern uint16   local_port;	                              /* ��λ���˿ں� */
extern uint8    subnet[4]  ;												      /*����W5500Ĭ����������*/
extern uint8    gateway[4] ;													    /*����W5500Ĭ������*/

extern uint8    local_ip2[4];										          /*������λ��W5500Ĭ��IP��ַ*/
extern uint16   local_port2;	                            /* ��λ���˿ں� */
extern uint8    subnet2[4]  ;												      /*����W5500Ĭ����������*/
extern uint8    gateway2[4] ;													    /*����W5500Ĭ������*/


#define W5500_SPI_SCK_PIN                   GPIO_PIN_12                            
#define W5500_SPI_MISO_PIN                  GPIO_PIN_13                  
#define W5500_SPI_MOSI_PIN                  GPIO_PIN_14                             
#define W5500_CS_PIN                        GPIO_PIN_11             
#define W5500_PORT                          GPIOE 


#define W5500_CS_LOW()      {GPIOE->BSRRH=W5500_CS_PIN;}
#define W5500_CS_HIGH()     {GPIOE->BSRRL=W5500_CS_PIN;}
				

#define W5500TOP_SPI_SCK_PIN                   GPIO_PIN_13                         
#define W5500TOP_SPI_MISO_PIN                  GPIO_PIN_14                 
#define W5500TOP_SPI_MOSI_PIN                  GPIO_PIN_15                            
#define W5500TOP_CS_PIN                        GPIO_PIN_12  
#define W5500TOP_PORT                          GPIOB
                  
#define W5500TOP_CS_LOW()      {GPIOB->BSRRH=W5500TOP_CS_PIN;}
#define W5500TOP_CS_HIGH()     {GPIOB->BSRRL=W5500TOP_CS_PIN;}						


#define W5500_SPIx					    	SPI4
#define W5500_SPIx_CLK_ENABLE()				__HAL_RCC_SPI4_CLK_ENABLE()

#define W5500_SPIx_FORCE_RESET()			__HAL_RCC_SPI4_FORCE_RESET()
#define W5500_SPIx_RELEASE_RESET()			__HAL_RCC_SPI4_RELEASE_RESET()

#define W5500_SPIx_SCK_CLK_ENABLE()			__HAL_RCC_GPIOE_CLK_ENABLE()
#define W5500_SPIx_SCK_GPIO					GPIOE
#define W5500_SPIx_SCK_PIN					GPIO_PIN_12
#define W5500_SPIx_SCK_AF					GPIO_AF5_SPI4

#define W5500_SPIx_MISO_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define W5500_SPIx_MISO_GPIO				GPIOE
#define W5500_SPIx_MISO_PIN 				GPIO_PIN_13
#define W5500_SPIx_MISO_AF					GPIO_AF5_SPI4

#define W5500_SPIx_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define W5500_SPIx_MOSI_GPIO				GPIOE
#define W5500_SPIx_MOSI_PIN 				GPIO_PIN_14
#define W5500_SPIx_MOSI_AF					GPIO_AF5_SPI4

#define W5500_SPIx_CS_CLK_ENABLE()			__HAL_RCC_GPIOE_CLK_ENABLE()
#define W5500_SPIx_CS_GPIO					GPIOE
#define W5500_SPIx_CS_PIN                   GPIO_PIN_11 
#define W5500_SPIx_CS_AF					GPIO_AF5_SPI4

#define W5500_SPIx_CS_LOW()      	GPIOE->BSRR=((uint32_t)W5500_SPIx_CS_PIN << 16U)
#define W5500_SPIx_CS_HIGH()     	GPIOE->BSRR=(W5500_SPIx_CS_PIN & 0xFFFF)



#define W5500_TOP_SPIx					    	SPI2
#define W5500_TOP_SPIx_CLK_ENABLE()				__HAL_RCC_SPI2_CLK_ENABLE()

#define W5500_TOP_SPIx_FORCE_RESET()			__HAL_RCC_SPI2_FORCE_RESET()
#define W5500_TOP_SPIx_RELEASE_RESET()			__HAL_RCC_SPI2_RELEASE_RESET()

#define W5500_TOP_SPIx_SCK_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()
#define W5500_TOP_SPIx_SCK_GPIO					GPIOB
#define W5500_TOP_SPIx_SCK_PIN					GPIO_PIN_13
#define W5500_TOP_SPIx_SCK_AF					GPIO_AF5_SPI2

#define W5500_TOP_SPIx_MISO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define W5500_TOP_SPIx_MISO_GPIO				GPIOB
#define W5500_TOP_SPIx_MISO_PIN 				GPIO_PIN_14
#define W5500_TOP_SPIx_MISO_AF					GPIO_AF5_SPI2

#define W5500_TOP_SPIx_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define W5500_TOP_SPIx_MOSI_GPIO				GPIOB
#define W5500_TOP_SPIx_MOSI_PIN 				GPIO_PIN_15
#define W5500_TOP_SPIx_MOSI_AF					GPIO_AF5_SPI2

#define W5500_TOP_SPIx_CS_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()
#define W5500_TOP_SPIx_CS_GPIO					GPIOB
#define W5500_TOP_SPIx_CS_PIN                   GPIO_PIN_12 
#define W5500_TOP_SPIx_CS_AF					GPIO_AF5_SPI2

#define W5500_SPIx_TOP_CS_LOW()      GPIOB->BSRR = ((uint32_t)W5500_TOP_SPIx_CS_PIN << 16U)
#define W5500_SPIx_TOP_CS_HIGH()     GPIOB->BSRR = (W5500_TOP_SPIx_CS_PIN & 0xFFFF)




/*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];																							/*MAC��ַ*/
  uint8 lip[4];																							/*local IP����IP��ַ*/
  uint8 sub[4];																							/*��������*/
  uint8 gw[4];																							/*����*/	
  uint8 dns[4];																							/*DNS��������ַ*/
  uint8 rip[4];																							/*remote IPԶ��IP��ַ*/
}CONFIG_MSG;

extern CONFIG_MSG  	ConfigMsg;


/*W5500SPI��غ���*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*��W5500����һ��8λ����*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/

void IINCHIP_WRITE_TOP( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
uint8 IINCHIP_READ_TOP(uint32 addrbsb);													/*��W5500����һ��8λ����*/
uint16 wiz_write_buf_top(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
uint16 wiz_read_buf_top(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/

void w5500_init(void);
void w5500_top_init(void);
extern uint8 mac2[6];


#endif
