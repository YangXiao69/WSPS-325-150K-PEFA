#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_
#include "stm32f4xx.h"
#include "system.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															/*远端IP地址*/
extern uint8    remote_ip2[4];
extern uint16 	remote_port;															/*远端端口号*/
extern uint16   remote_port2;
extern uint16		local_port;																/*定义本地端口*/

extern uint8    local_ip[4];										          /*定义上位机W5500默认IP地址*/
extern uint16   local_port;	                              /* 上位机端口号 */
extern uint8    subnet[4]  ;												      /*定义W5500默认子网掩码*/
extern uint8    gateway[4] ;													    /*定义W5500默认网关*/

extern uint8    local_ip2[4];										          /*定义上位机W5500默认IP地址*/
extern uint16   local_port2;	                            /* 上位机端口号 */
extern uint8    subnet2[4]  ;												      /*定义W5500默认子网掩码*/
extern uint8    gateway2[4] ;													    /*定义W5500默认网关*/


#define W5500_SPI_SCK_PIN                   GPIO_Pin_12                            
#define W5500_SPI_MISO_PIN                  GPIO_Pin_13                  
#define W5500_SPI_MOSI_PIN                  GPIO_Pin_14                             
#define W5500_CS_PIN                        GPIO_Pin_11             
                  
#define W5500_CS_LOW()      {GPIOE->BSRRH=W5500_CS_PIN;}
#define W5500_CS_HIGH()     {GPIOE->BSRRL=W5500_CS_PIN;}
				

#define W5500TOP_SPI_SCK_PIN                   GPIO_Pin_13                         
#define W5500TOP_SPI_MISO_PIN                  GPIO_Pin_14                 
#define W5500TOP_SPI_MOSI_PIN                  GPIO_Pin_15                            
#define W5500TOP_CS_PIN                        GPIO_Pin_12             
                  
#define W5500TOP_CS_LOW()      {GPIOB->BSRRH=W5500TOP_CS_PIN;}
#define W5500TOP_CS_HIGH()     {GPIOB->BSRRL=W5500TOP_CS_PIN;}						


/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];																							/*MAC地址*/
  uint8 lip[4];																							/*local IP本地IP地址*/
  uint8 sub[4];																							/*子网掩码*/
  uint8 gw[4];																							/*网关*/	
  uint8 dns[4];																							/*DNS服务器地址*/
  uint8 rip[4];																							/*remote IP远程IP地址*/
}CONFIG_MSG;

extern CONFIG_MSG  	ConfigMsg;


/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

void IINCHIP_WRITE_TOP( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ_TOP(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf_top(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf_top(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

void w5500_init(void);
void w5500_top_init(void);
extern uint8 mac2[6];


#endif
