#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															/*远端IP地址*/
extern uint16 	remote_port;															/*远端端口号*/
extern uint16		local_port;																/*定义本地端口*/

extern u8 NumName;
/*定义SPI1作为W5500的硬件接口*/
#define WIZ_SCS									GPIO_Pin_12								/*定义W5500的片选管脚*/
#define WIZ_SCLK								GPIO_Pin_13								/*定义W5500的时钟管脚*/
#define WIZ_MISO								GPIO_Pin_14								/*定义W5500的MISO管脚*/
#define WIZ_MOSI								GPIO_Pin_15								/*定义W5500的MOSI管脚*/
#define WIZ_RESET							  GPIO_Pin_9								/*定义W5500的RESET管脚*/

/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG
{
    uint8 mac[6];																							/*MAC地址*/
    uint8 lip[4];																							/*local IP本地IP地址*/
    uint8 sub[4];																							/*子网掩码*/
    uint8 gw[4];																							/*网关*/
    uint8 dns[4];																							/*DNS服务器地址*/
    uint8 rip[4];																							/*remote IP远程IP地址*/
} CONFIG_MSG;

extern CONFIG_MSG  	ConfigMsg;

/*MCU配置相关函数*/
void spi_w5500_config(void);												    		/*SPI接口reset 及中断引脚*/
void timer2_init(void);																			/*STM32定时器2初始化*/
void timer2_isr(void);																			/*定时器中断执行的函数*/
void reboot(void);																					/*STM32软复位*/


/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/
void w5500_init(void);

#endif
