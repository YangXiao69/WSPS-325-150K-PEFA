#include "./bsp/bsp.h"
#include "system.h"


//��ʼ��IIC
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIOBʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7); //PB6,PB7 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT(); //sda�����
    IIC_SDA(1);
    IIC_SCL(1);
    delay_us(4);
    IIC_SDA(0); //START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL(0); //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT(); //sda�����
    IIC_SCL(0);
    IIC_SDA(0); //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL(1);
    IIC_SDA(1); //����I2C���߽����ź�
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN(); //SDA����Ϊ����
    IIC_SDA(1);
    delay_us(1);
    IIC_SCL(1);
    delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL(0); //ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(0);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}
//������ACKӦ��
void IIC_NAck(void)
{
    IIC_SCL(0);
    SDA_OUT();
    IIC_SDA(1);
    delay_us(2);
    IIC_SCL(1);
    delay_us(2);
    IIC_SCL(0);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t,b;
    SDA_OUT();
    IIC_SCL(0); //����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        b = (txd & 0x80) >> 7;
        IIC_SDA(b);
        txd <<= 1;
        delay_us(2); //��TEA5767��������ʱ���Ǳ����
        IIC_SCL(1);
        delay_us(2);
        IIC_SCL(0);
        delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); //SDA����Ϊ����
    for (i = 0; i < 8; i++)
    {
        IIC_SCL(0);
        delay_us(2);
        IIC_SCL(1);
        receive <<= 1;
        if (READ_SDA)
            receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck(); //����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
    IIC_Init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA0); //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr >> 8); //���͸ߵ�ַ
        IIC_Wait_Ack();
    }
    else
        IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); //����������ַ0XA0,д����

    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr % 256); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0XA1); //�������ģʽ
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop(); //����һ��ֹͣ����
    return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
    IIC_Start();
    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA0); //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr >> 8); //���͸ߵ�ַ
    }
    else
    {
        IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); //����������ַ0XA0,д����
    }
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr % 256); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite); //�����ֽ�
    IIC_Wait_Ack();
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr, u32 DataToWrite, uint8_t Len)
{
    uint8_t t;
    for (t = 0; t < Len; t++)
    {
        AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
{
    uint8_t t;
    u32 temp = 0;
    for (t = 0; t < Len; t++)
    {
        temp <<= 8;
        temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
    }
    return temp;
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Check(void)
{
    uint8_t temp;
    temp = AT24CXX_ReadOneByte(255); //����ÿ�ο�����дAT24CXX
    if (temp == 0X55)
        return 0;
    else //�ų���һ�γ�ʼ�������
    {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if (temp == 0X55)
            return 0;
    }
    return 1;
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    while (NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

void AT24CXX_WriteTwoByte(uint8_t WriteAddr, uint16_t DataToWrite)
{
    AT24CXX_WriteOneByte(WriteAddr, DataToWrite >> 8);
    AT24CXX_WriteOneByte(WriteAddr + 1, DataToWrite);
}
