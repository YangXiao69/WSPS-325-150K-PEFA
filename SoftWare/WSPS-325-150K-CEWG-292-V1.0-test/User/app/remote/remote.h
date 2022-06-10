#ifndef __REMOTE_H
#define	__REMOTE_H


#include "stm32f4xx.h"



extern uint8_t CalibFlag;
extern u8 Calib_Tx_buff[4];

void remote_W5500_reseive(void);
void remote_RS485_reseive(void);
void remote_RS232_reseive(void);   // 01 06  00 01  00 01  CRC
void work_data_clean(u8 num);
void work_data_stor(u8 num,u8 *buff);

void remote_RJ45_reseive(void);
extern u16 Modbus[2048];

void lcd2_send_one(u16 data,u16 adrss) ;
void lcd2_send_two(u32 data,u16 adrss) ;
void LCD2_receive(void);
#endif /* __ADC_H */

