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
    
//IO��������
#define IIC_SCL(p) GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)p) //SCL
#define IIC_SDA(p) GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)p) //SDA
#define READ_SDA GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) //����SDA

//IIC���в�������
void IIC_Init(void);                 //��ʼ��IIC��IO��
void IIC_Start(void);                //����IIC��ʼ�ź�
void IIC_Stop(void);                 //����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);          //IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void);               //IIC�ȴ�ACK�ź�
void IIC_Ack(void);                  //IIC����ACK�ź�
void IIC_NAck(void);                 //IIC������ACK�ź�

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
#define EE_TYPE AT24C02 //Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);                              //ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);          //ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(uint16_t WriteAddr, u32 DataToWrite, uint8_t Len); //ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len);                     //ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);    //��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);       //��ָ����ַ��ʼ����ָ�����ȵ�����

uint8_t AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC
#endif