#ifndef __BSP_24CXX_H
#define __BSP_24CXX_H

#include "./bsp/bsp.h"
#include "stm32f10x.h"


#define SDA_IN()                    \
    {                               \
        GPIOB->CRL &= 0X0FFFFFFF;   \
        GPIOB->CRL |= (u32)8 << 28; \
    }
#define SDA_OUT()                   \
    {                               \
        GPIOB->CRL &= 0X0FFFFFFF;   \
        GPIOB->CRL |= (u32)3 << 28; \
    }
    
//IO操作函数
#define IIC_SCL(p) GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)p) //SCL
#define IIC_SDA(p) GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)p) //SDA
#define READ_SDA GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) //输入SDA

//IIC所有操作函数
void IIC_Init(void);                 //初始化IIC的IO口
void IIC_Start(void);                //发送IIC开始信号
void IIC_Stop(void);                 //发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);          //IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack); //IIC读取一个字节
uint8_t IIC_Wait_Ack(void);               //IIC等待ACK信号
void IIC_Ack(void);                  //IIC发送ACK信号
void IIC_NAck(void);                 //IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
void AT24CXX_WriteTwoByte(uint8_t WriteAddr, uint16_t DataToWrite);

#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767
#define EE_TYPE AT24C02 //Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);                              //指定地址读取一个字节
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);          //指定地址写入一个字节
void AT24CXX_WriteLenByte(uint16_t WriteAddr, u32 DataToWrite, uint8_t Len); //指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len);                     //指定地址开始读取指定长度数据
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);    //从指定地址开始写入指定长度的数据
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);       //从指定地址开始读出指定长度的数据

uint8_t AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC
#endif
