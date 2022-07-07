#include "lcd_rs232.h"
#include "task.h"
#include "usart.h"

uint8_t UART2_DMA_TX[64];
uint8_t UART2_DMA_RX[64];

/*
 *  CRC校验
 */
uint16_t crc16_modbus(uint8_t *data, uint8_t length) // 0106000100179804
{
    uint8_t i = 0;
    uint16_t crc = 0xffff; // Initial value
    while (length--)
    {
        crc ^= *data++; // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001; // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

void USART2_Send(uint8_t len)
{
	HAL_UART_Transmit_DMA(&huart2, UART2_DMA_TX, len); 
}


void Task_DC_Read_Data(void)
{
    uint16_t CRCNum;
    static uint8_t PowerAddr;
    static uint16_t i = 0;
    if(i >= 5000)
    {
        i = 0;
        if (PowerAddr >= 7)     /*8个电源插件*/
        {
            PowerAddr = 1;
        }
        UART2_DMA_TX[0] = PowerAddr;
        UART2_DMA_TX[1] = 0x03;
        UART2_DMA_TX[2] = 0x00;
        UART2_DMA_TX[3] = 0x00;
        UART2_DMA_TX[4] = 0x00;
        UART2_DMA_TX[5] = 0x07; 
        CRCNum = crc16_modbus((uint8_t *)UART2_DMA_TX, 6);
        UART2_DMA_TX[6] = CRCNum;
        UART2_DMA_TX[7] = CRCNum >> 8;
        USART2_Send(8);
        
        PowerAddr++;
        

    }
    i++;
}











