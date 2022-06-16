#ifndef  __NETWORK_H
#define  __NETWORK_H


#include "stm32f4xx.h"


extern uint8_t g_ucUdpReady;
extern uint8_t RJ45_Rx_buff[64];


void Write_ModbusReg(void);

void UDP_Service(void);
void TCP_TopServer(void);


void UDP_SendSettings(void);

#endif


