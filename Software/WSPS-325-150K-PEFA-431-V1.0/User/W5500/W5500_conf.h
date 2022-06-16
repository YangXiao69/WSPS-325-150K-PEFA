#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															/*Զ��IP��ַ*/
extern uint16 	remote_port;															/*Զ�˶˿ں�*/
extern uint16		local_port;																/*���屾�ض˿�*/

extern u8 NumName;
/*����SPI1��ΪW5500��Ӳ���ӿ�*/
#define WIZ_SCS									GPIO_Pin_12								/*����W5500��Ƭѡ�ܽ�*/
#define WIZ_SCLK								GPIO_Pin_13								/*����W5500��ʱ�ӹܽ�*/
#define WIZ_MISO								GPIO_Pin_14								/*����W5500��MISO�ܽ�*/
#define WIZ_MOSI								GPIO_Pin_15								/*����W5500��MOSI�ܽ�*/
#define WIZ_RESET							  GPIO_Pin_9								/*����W5500��RESET�ܽ�*/

/*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
typedef struct _CONFIG_MSG
{
    uint8 mac[6];																							/*MAC��ַ*/
    uint8 lip[4];																							/*local IP����IP��ַ*/
    uint8 sub[4];																							/*��������*/
    uint8 gw[4];																							/*����*/
    uint8 dns[4];																							/*DNS��������ַ*/
    uint8 rip[4];																							/*remote IPԶ��IP��ַ*/
} CONFIG_MSG;

extern CONFIG_MSG  	ConfigMsg;

/*MCU������غ���*/
void spi_w5500_config(void);												    		/*SPI�ӿ�reset ���ж�����*/
void timer2_init(void);																			/*STM32��ʱ��2��ʼ��*/
void timer2_isr(void);																			/*��ʱ���ж�ִ�еĺ���*/
void reboot(void);																					/*STM32��λ*/


/*W5500SPI��غ���*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*��W5500����һ��8λ����*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/

/*W5500����������غ���*/
void reset_w5500(void);																			/*Ӳ��λW5500*/
void set_w5500_mac(void);																		/*����W5500��MAC��ַ*/
void set_w5500_ip(void);																		/*����W5500��IP��ַ*/
void w5500_init(void);

#endif
