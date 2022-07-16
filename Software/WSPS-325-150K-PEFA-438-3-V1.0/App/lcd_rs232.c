#include "lcd_rs232.h"
#include "task.h"
#include "usart.h"
#include "tcp.h"

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


void work_232send_buff(uint16_t addr,uint8_t len,uint8_t way)
{
    uint16_t i,crc; 
	
    if(len>120) 
        return;
    		
    if(way==0x03)                                                     //读
    {
        UART2_DMA_TX[0] = UART2_DMA_RX[0];
        UART2_DMA_TX[1] = way;
        UART2_DMA_TX[2]=len*2; 	
        for(i=0;i< len;i++)
        {
            UART2_DMA_RX[2*i+3] = Modbus[i+addr]>>8;
            UART2_DMA_RX[2*i+4] = Modbus[i+addr];
        }
        crc=crc16_modbus(UART2_DMA_TX,3+len*2);
        UART2_DMA_TX[3+len*2] = crc;
        UART2_DMA_TX[4+len*2] = crc>>8;
        USART2_Send(5+len*2);				
    }
    else if(way==0x06)                                              //写
    {
        UART2_DMA_TX[0] = UART2_DMA_RX[0];
        UART2_DMA_TX[1] = way;
        UART2_DMA_TX[2] = addr>>8; 
        UART2_DMA_TX[3] = addr;	
        UART2_DMA_TX[4] = Modbus[addr]>>8;
        UART2_DMA_TX[5] = Modbus[addr];				
        crc=crc16_modbus(UART2_DMA_RX,6);
        UART2_DMA_TX[6] = crc;
        UART2_DMA_TX[7] = crc>>8;
        USART2_Send(8);						
    }
    else if(way==0x10)                                             //写
    {
        UART2_DMA_TX[0] = UART2_DMA_RX[0];
        UART2_DMA_TX[1] = way;
        UART2_DMA_TX[2]=addr>>8; 
        UART2_DMA_TX[3]=addr;	
        UART2_DMA_TX[4] = 0;  	 
        UART2_DMA_TX[5] = len;					  
        crc=crc16_modbus(UART2_DMA_TX,6);
        UART2_DMA_TX[6] = crc;
        UART2_DMA_TX[7] = crc>>8;	
        USART2_Send(8);		
    }	
}

void Modbus_RS232_reseive(void)
{
    uint16_t Data = 0, ADDR = 0,i,mNum,mData;         


    if (UART2_DMA_RX[0] == System.Modbus_Addr || UART2_DMA_RX[0] == 0x00)
    {
        if (UART2_DMA_RX[1] == 0x06) /* 写指令 */
        {
            ADDR = ((uint16_t)UART2_DMA_RX[2] << 8) + UART2_DMA_RX[3];   //访问地址
            Data = ((uint16_t)UART2_DMA_RX[4] << 8) + UART2_DMA_RX[5]; //写数据 或者 读取长度
            Modbus_Progress(ADDR,Data);
            work_232send_buff(ADDR, 1, 0x06);
        }
        else if (UART2_DMA_RX[1] == 0x03) /*读指令*/
        {
            ADDR = ((uint16_t)UART2_DMA_RX[2] << 8) + UART2_DMA_RX[3];   //访问地址
            Data = ((uint16_t)UART2_DMA_RX[4] << 8) + UART2_DMA_RX[5]; //写数据 或者 读取长度
            work_232send_buff(ADDR, Data, 0x03);
        }
        else if (UART2_DMA_RX[1] == 0x10) //连续写
        {
            ADDR = ((uint16_t)UART2_DMA_RX[2] << 8) + UART2_DMA_RX[3];   //访问地址
            Data = ((uint16_t)UART2_DMA_RX[4] << 8) + UART2_DMA_RX[5]; //写数据 或者 读取长度
            for(i=0;i<Data;i++)
            {
                mData = ((uint16_t)UART2_DMA_RX[7+i*2] << 8) + UART2_DMA_RX[8+i*2];
                Modbus_Progress(ADDR,mData);
                ADDR++;
            }
            Data = ((uint16_t)UART2_DMA_RX[4] << 8) + UART2_DMA_RX[5]; //写数据 或者 读取长度            
            work_232send_buff(ADDR, Data, 0x10);
        }
    }
}











