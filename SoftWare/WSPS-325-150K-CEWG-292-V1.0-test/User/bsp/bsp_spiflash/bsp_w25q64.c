#include "system.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

static uint8_t s_spiBuf[4*1024];

u32 FLASH_WriteAddr = 0x000000;
u32 FLASH_ReadAddr = 0x000000;

void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);
  FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
 
  //�������Ÿ���
  GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,FLASH_SPI_SCK_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,FLASH_SPI_MISO_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,FLASH_SPI_MOSI_AF); 
  
  /*!< ���� SPI_FLASH_SPI ����: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< ���� SPI_FLASH_SPI ����: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);

  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI_FLASH_CS_HIGH();

  /* FLASH_SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI, &SPI_InitStructure);

  /* ʹ�� FLASH_SPI  */
  SPI_Cmd(FLASH_SPI, ENABLE);

}

 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  SPI_FLASH_WriteEnable();                 //����FLASHдʹ������ 
  SPI_FLASH_WaitForWriteEnd();             //�ȴ��������

  SPI_FLASH_CS_LOW();                      //Ƭѡ
  SPI_FLASH_SendByte(W25X_SectorErase);    //������������ָ��

  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);  //���Ͳ���������ַ�ĸ�λ
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);     //���Ͳ���������ַ����λ
  SPI_FLASH_SendByte(SectorAddr & 0xFF);              //���Ͳ���������ַ�ĵ�λ

  SPI_FLASH_CS_HIGH();               //ֹͣ�ź� FLASH: CS �ߵ�ƽ
  SPI_FLASH_WaitForWriteEnd();       //�ȴ��������
}


 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{
  
  SPI_FLASH_WriteEnable();   //����FLASHдʹ������ 
  SPI_FLASH_CS_LOW();

  SPI_FLASH_SendByte(W25X_ChipErase);  /* �����������ָ��*/
 
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  
  SPI_FLASH_CS_LOW();                                /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_SendByte(W25X_PageProgram);              /* дҳдָ��*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);  /*����д��ַ�ĸ�λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);     /*����д��ַ����λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);              /*����д��ַ�ĵ�λ*/

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  
  SPI_FLASH_CS_LOW();/* ѡ��FLASH: CS�͵�ƽ */

  SPI_FLASH_SendByte(W25X_ReadData);                /* ���� �� ָ�� */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);      /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);              /* ���� �� ��ַ��λ */

  while (NumByteToRead--)/* ��ȡ���� */
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  SPI_FLASH_CS_LOW();                           /* ��ʼͨѶ��CS�͵�ƽ */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);       /* ����JEDECָ���ȡID */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);       /* ��ȡһ���ֽ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);       /* ��ȡһ���ֽ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);       /* ��ȡһ���ֽ����� */
  SPI_FLASH_CS_HIGH();                          /* ֹͣͨѶ��CS�ߵ�ƽ */
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;  /*�����������������Ϊ�����ķ���ֵ*/
  return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}

void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(FLASH_SPI, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(FLASH_SPI);
}


u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }

  /* Send Half Word through the FLASH_SPI peripheral */
  SPI_I2S_SendData(FLASH_SPI, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(FLASH_SPI);
}


 /**
  * @brief  ��FLASH���� дʹ�� ����
  */
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();                   /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_SendByte(W25X_WriteEnable); /* ����дʹ������*/
  SPI_FLASH_CS_HIGH();                  /*ͨѶ������CS�� */
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();                     /* ѡ�� FLASH: CS �� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg); /* ���� ��״̬�Ĵ��� ���� */
  SPITimeout = SPIT_FLAG_TIMEOUT;
  do                                      /* ��FLASHæµ����ȴ� */
  {
    
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 /* ��ȡFLASHоƬ��״̬�Ĵ��� */

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */
  SPI_FLASH_CS_HIGH();/* ֹͣ�ź�  FLASH: CS �� */
}


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  SPI_FLASH_CS_LOW();/* ѡ�� FLASH: CS �� */
  SPI_FLASH_SendByte(W25X_PowerDown);/* ���� ���� ���� */
  SPI_FLASH_CS_HIGH();/* ֹͣ�ź�  FLASH: CS �� */
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  SPI_FLASH_CS_LOW();/*ѡ�� FLASH: CS �� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);/* ���� �ϵ� ���� */
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   


/*
  * @brief  �ȴ���ʱ�ص�����
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  return 0;
}
   





/*
        src Դ�ַ������׵�ַ(buf�ĵ�ַ) 
        separator ָ���ķָ��ַ�
        dest �������ַ���������
        num �ָ�����ַ����ĸ���
*/



void split(char *src, const char *separator, char **dest, int *num) 
{

     char *pNext;
     int count = 0;
     
     if (src == NULL || strlen(src) == 0) //�������ĵ�ַΪ�ջ򳤶�Ϊ0��ֱ����ֹ 
        return;
        
     if (separator == NULL || strlen(separator) == 0) //��δָ���ָ���ַ�����ֱ����ֹ 
        return;
        
     pNext = (char *)strtok(src,separator); //����ʹ��(char *)����ǿ������ת��(��Ȼ��д�еı������в������ָ�����)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //����ʹ��(char *)����ǿ������ת��
    }  
    *num = count;
}     


uint8_t bsp_spiRead1(void)
{
	/* Ӳ��SPI */
	uint8_t read;

	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI5, 0);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	read = SPI_I2S_ReceiveData(SPI5);

	/* ���ض��������� */
	return read;

}

void bsp_spiWrite1(uint8_t _ucByte)
{

	/* Ӳ��SPI */
	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI5, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	SPI_I2S_ReceiveData(SPI5);

}




static void sf_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();                     /* ѡ�� FLASH: CS �� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg); /* ���� ��״̬�Ĵ��� ���� */
  SPITimeout = SPIT_FLAG_TIMEOUT;
  do                                      /* ��FLASHæµ����ȴ� */
  {
    
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 /* ��ȡFLASHоƬ��״̬�Ĵ��� */

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */
  SPI_FLASH_CS_HIGH();/* ֹͣ�ź�  FLASH: CS �� */
}


static void sf_WriteEnable(void)
{
    SPI_FLASH_CS_LOW();                   /* ͨѶ��ʼ��CS�� */
    SPI_FLASH_SendByte(W25X_WriteEnable); /* ����дʹ������*/
    SPI_FLASH_CS_HIGH();                  /*ͨѶ������CS�� */	
}


void sf_EraseSector(uint32_t _uiSectorAddr)
{
  SPI_FLASH_WriteEnable();                 //����FLASHдʹ������ 
  SPI_FLASH_WaitForWriteEnd();             //�ȴ��������

  SPI_FLASH_CS_LOW();                      //Ƭѡ
  SPI_FLASH_SendByte(W25X_SectorErase);    //������������ָ��

  SPI_FLASH_SendByte((_uiSectorAddr & 0xFF0000) >> 16);  //���Ͳ���������ַ�ĸ�λ
  SPI_FLASH_SendByte((_uiSectorAddr & 0xFF00) >> 8);     //���Ͳ���������ַ����λ
  SPI_FLASH_SendByte(_uiSectorAddr & 0xFF);              //���Ͳ���������ַ�ĵ�λ

  SPI_FLASH_CS_HIGH();               //ֹͣ�ź� FLASH: CS �ߵ�ƽ
  SPI_FLASH_WaitForWriteEnd();       //�ȴ��������
}


void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > SPI_FLASH_TotalSize)
	{
		return;
	}

	/* ������������ */
	SPI_FLASH_CS_LOW();									/* ʹ��Ƭѡ */
	bsp_spiWrite1(W25X_ReadData);							/* ���Ͷ����� */
	bsp_spiWrite1((_uiReadAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
	bsp_spiWrite1((_uiReadAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	bsp_spiWrite1(_uiReadAddr & 0xFF);				/* ����������ַ��8bit */
	while (_uiSize--)
	{
		*_pBuf++ = bsp_spiRead1();			/* ��һ���ֽڲ��洢��pBuf�������ָ���Լ�1 */
	}
	SPI_FLASH_CS_HIGH();									/* ����Ƭѡ */
}


static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;


	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
	return 0;
}

void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize)
{
	uint32_t i, j;


    for (j = 0; j < _usSize / 256; j++)
    {
        sf_WriteEnable();								/* ����дʹ������ */
    
        SPI_FLASH_CS_LOW();									/* ʹ��Ƭѡ */
        bsp_spiWrite1(0x02);								/* ����AAI����(��ַ�Զ����ӱ��) */
        bsp_spiWrite1((_uiWriteAddr & 0xFF0000) >> 16);	/* ����������ַ�ĸ�8bit */
        bsp_spiWrite1((_uiWriteAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
        bsp_spiWrite1(_uiWriteAddr & 0xFF);				/* ����������ַ��8bit */
    
        for (i = 0; i < 256; i++)
        {
            bsp_spiWrite1(*_pBuf++);					/* �������� */
        }
    
        SPI_FLASH_CS_HIGH();								/* ��ֹƬѡ */
    
        sf_WaitForWriteEnd();						/* �ȴ�����Flash�ڲ�д������� */
    
        _uiWriteAddr += 256;
    }
    
    /* ����д����״̬ */
    SPI_FLASH_CS_LOW();
    bsp_spiWrite1(W25X_WriteDisable);
    SPI_FLASH_CS_HIGH();
    
    sf_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
	
}


static uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint8_t ucValue;

	/* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
	if ((_uiSrcAddr + _uiSize) > SPI_FLASH_TotalSize)
	{
		return 1;
	}

	if (_uiSize == 0)
	{
		return 0;
	}

	SPI_FLASH_CS_LOW();									/* ʹ��Ƭѡ */
	bsp_spiWrite1(W25X_ReadData);							/* ���Ͷ����� */
	bsp_spiWrite1((_uiSrcAddr & 0xFF0000) >> 16);		/* ����������ַ�ĸ�8bit */
	bsp_spiWrite1((_uiSrcAddr & 0xFF00) >> 8);		/* ����������ַ�м�8bit */
	bsp_spiWrite1(_uiSrcAddr & 0xFF);					/* ����������ַ��8bit */
	while (_uiSize--)
	{
		/* ��һ���ֽ� */
		ucValue = bsp_spiRead1();
		if (*_ucpTar++ != ucValue)
		{
			SPI_FLASH_CS_HIGH();
			return 1;
		}
	}
	SPI_FLASH_CS_HIGH();
	return 0;
}





static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen)
{
	uint16_t i;
	uint16_t j;					/* ������ʱ */
	uint32_t uiFirstAddr;		/* ������ַ */
	uint8_t ucNeedErase;		/* 1��ʾ��Ҫ���� */
	uint8_t cRet;

	/* ����Ϊ0ʱ����������,ֱ����Ϊ�ɹ� */
	if (_usWrLen == 0)
	{
		return 1;
	}

	/* ���ƫ�Ƶ�ַ����оƬ�������˳� */
	if (_uiWrAddr >= SPI_FLASH_TotalSize)
	{
		return 0;
	}

	/* ������ݳ��ȴ����������������˳� */
	if (_usWrLen > SPI_FLASH_PageSize)
	{
		return 0;
	}

	/* ���FLASH�е�����û�б仯,��дFLASH */
	sf_ReadBuffer(s_spiBuf, _uiWrAddr, _usWrLen);
	if (memcmp(s_spiBuf, _ucpSrc, _usWrLen) == 0)
	{
		return 1;
	}

	/* �ж��Ƿ���Ҫ�Ȳ������� */
	/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
	ucNeedErase = 0;
	if (sf_NeedErase(s_spiBuf, _ucpSrc, _usWrLen))
	{
		ucNeedErase = 1;
	}

	uiFirstAddr = _uiWrAddr & (~(SPI_FLASH_PageSize - 1));

	if (_usWrLen == SPI_FLASH_PageSize)		/* ������������д */
	{
		for	(i = 0; i < SPI_FLASH_PageSize; i++)
		{
			s_spiBuf[i] = _ucpSrc[i];
		}
	}
	else						/* ��д�������� */
	{
		/* �Ƚ��������������ݶ��� */
		sf_ReadBuffer(s_spiBuf, uiFirstAddr, SPI_FLASH_PageSize);

		/* ���������ݸ��� */
		i = _uiWrAddr & (SPI_FLASH_PageSize - 1);
		memcpy(&s_spiBuf[i], _ucpSrc, _usWrLen);
	}

	/* д��֮�����У�飬�������ȷ����д�����3�� */
	cRet = 0;
	for (i = 0; i < 3; i++)
	{

		/* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
		if (ucNeedErase == 1)
		{
			sf_EraseSector(uiFirstAddr);		/* ����1������ */
		}

		/* ���һ��PAGE */
		sf_PageWrite(s_spiBuf, uiFirstAddr, SPI_FLASH_PageSize);

		if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
		{
			cRet = 1;
			break;
		}
		else
		{
			if (sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0)
			{
				cRet = 1;
				break;
			}

			/* ʧ�ܺ��ӳ�һ��ʱ�������� */
			for (j = 0; j < 10000; j++);
		}
	}

	return cRet;
}



uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = _uiWriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage =  _usWriteSize / SPI_FLASH_PageSize;
	NumOfSingle = _usWriteSize % SPI_FLASH_PageSize;

	if (Addr == 0) /* ��ʼ��ַ��ҳ���׵�ַ  */
	{
		if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
		{
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
			{
				return 0;
			}
		}
		else 	/* ���ݳ��ȴ��ڵ���ҳ���С */
		{
			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, SPI_FLASH_PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  SPI_FLASH_PageSize;
				_pBuf += SPI_FLASH_PageSize;
			}
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
			{
				return 0;
			}
		}
	}
	else  /* ��ʼ��ַ����ҳ���׵�ַ  */
	{
		if (NumOfPage == 0) /* ���ݳ���С��ҳ���С */
		{
			if (NumOfSingle > count) /* (_usWriteSize + _uiWriteAddr) > SPI_FLASH_PAGESIZE */
			{
				temp = NumOfSingle - count;

				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
				{
					return 0;
				}

				_uiWriteAddr +=  count;
				_pBuf += count;

				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, temp) == 0)
				{
					return 0;
				}
			}
			else
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
				{
					return 0;
				}
			}
		}
		else	/* ���ݳ��ȴ��ڵ���ҳ���С */
		{
			_usWriteSize -= count;
			NumOfPage =  _usWriteSize / SPI_FLASH_PageSize;
			NumOfSingle = _usWriteSize % SPI_FLASH_PageSize;

			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, count) == 0)
			{
				return 0;
			}

			_uiWriteAddr +=  count;
			_pBuf += count;

			while (NumOfPage--)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, SPI_FLASH_PageSize) == 0)
				{
					return 0;
				}
				_uiWriteAddr +=  SPI_FLASH_PageSize;
				_pBuf += SPI_FLASH_PageSize;
			}

			if (NumOfSingle != 0)
			{
				if (sf_AutoWritePage(_pBuf, _uiWriteAddr, NumOfSingle) == 0)
				{
					return 0;
				}
			}
		}
	}
	return 1;	/* �ɹ� */
}


/*
***********************************************************************
*@brief    
					
*@param		num: exception number.
*@param		*code: exception number.
*@param		*ts:	exception timestamp.
*@return  
**************************************************************************
*/
//void wtErrToFlash(u16 num,u16 code,u8 time[5])
//{
//	char numstr[10];
//	char codestr[10] = "";
//	char Tx_Buffer[50] = "";
//	
//	u32 FLASH_WriteAddress = (num%errcnt) * SPI_FLASH_PageSize;                        
//	
//	sprintf(numstr, "%d", num);
//	sprintf(codestr, "%d", code); 
//	strcat(numstr,",");
//	strcat(codestr,","); 

//	strcat(Tx_Buffer,numstr);
//	strcat(Tx_Buffer,codestr);
//	strcat(Tx_Buffer,(char *)time);
//	
//	
//	//printf("Tx_Buffer in SoreExceptionToFlash : %s\n",Tx_Buffer);
//	
//	
//	sf_WriteBuffer((u8*)Tx_Buffer,FLASH_WriteAddress,SPI_FLASH_RWPageSize);
//	
//}

static int strToHex(char* str)
{
    
    int result = 0;
	
    while (*str) 
    {
        if (*str >= '0' && *str <= '9')
            result = result * 16 + (*str - '0');
        else if (*str >= 'a' && *str <= 'f')
            result = result * 16 + (*str - 'a' + 10);
        else if (*str >= 'A' && *str <= 'F')
            result = result * 16 + (*str - 'A' + 10);
        else {
            return 0;
        }
        str++;
    }
		
    return result;
}


void wtErrToFlash(u16 num,u16 code,u8 time[5])
{
	uint8_t i = 0;
	uint16_t m,n;
	uint8_t Tx_Buffer[10];
	char numstr[5] = "";
	char codestr[5] = "";
	
	
	u32 FLASH_WriteAddress = (num%errcnt) * SPI_FLASH_PageSize;                        
	
	
	sprintf(numstr,"%x",num);
	sprintf(codestr,"%x",code);
	
//	printf("numstr %s \n",numstr);
//	printf("codestr %s \n",codestr);
	
	m = strToHex(numstr);
	n = strToHex(codestr);
	
	
//	printf("n: %x",n);
	
	
	Tx_Buffer[0] = (m >> 8) & 0xFF;
	Tx_Buffer[1] = m & 0xFF;
	
	Tx_Buffer[2] = (n >> 8) & 0xFF;
	Tx_Buffer[3] = n & 0xFF;
	
	
	
	
	
	for(i = 4; i < 10; i++)
	{
			Tx_Buffer[i] = time[i-4];
	}
	
//	printf("-----Tx_Buffer in wtErrToFlash --------\n");
//	
//	for(i = 0; i < 10; i++)
//	{
//		printf("Tx_Buffer %d:  %x\n",i,Tx_Buffer[i]);
//	}
	
	
	sf_WriteBuffer(Tx_Buffer,FLASH_WriteAddress,SPI_FLASH_RWPageSize);
	
}



void wtCmdToFlash(u16 num,u16 code, u16 cmdvalue, u8 *time)
{
	uint8_t i = 0;
//	uint16_t m = 0, n = 0;
	uint8_t Tx_Buffer[50] = {0};
//	char numstr[5] = "";
//	char codestr[5] = "";
	
	u32 FLASH_WriteAddress = num * SPI_FLASH_PageSize;                        
	
	
//	sprintf(numstr,"%x",num);
//	sprintf(codestr,"%x",code);
//	
////	printf("numstr %s \n",numstr);
////	printf("codestr %s \n",codestr);
//	
//	m = strToHex(numstr);
//	n = strToHex(codestr);
	
	
//	printf("n: %x",n);
	
	
//	Tx_Buffer[0] = (m >> 8) & 0xFF;
//	Tx_Buffer[1] = m & 0xFF;
//	
//	Tx_Buffer[2] = (n >> 8) & 0xFF;
//	Tx_Buffer[3] = n & 0xFF;


	Tx_Buffer[0] = (num >> 8) & 0xFF;
	Tx_Buffer[1] = num & 0xFF;

	Tx_Buffer[2] = (code >> 8) & 0xFF;
	Tx_Buffer[3] = code & 0xFF;



	
	Tx_Buffer[4] = cmdvalue >> 8;
	Tx_Buffer[5] = cmdvalue & 0xFF;	
	
	Tx_Buffer[6] = Modbus[0x00A1] >> 8;
	Tx_Buffer[7] = Modbus[0x00A1] & 0xFF;		
    
	Tx_Buffer[8] = Modbus[0x00A2] >> 8;
	Tx_Buffer[9] = Modbus[0x00A2] & 0xFF;	    
    
	Tx_Buffer[10] = Modbus[0x00B2] >> 8;
	Tx_Buffer[11] = Modbus[0x00B2] & 0xFF;	
    
	Tx_Buffer[12] = Modbus[0x00B5] >> 8;
	Tx_Buffer[13] = Modbus[0x00B5] & 0xFF;	    
    
	Tx_Buffer[14] = Modbus[0x00B4] >> 8;
	Tx_Buffer[15] = Modbus[0x00B4] & 0xFF;	    
    
	
	for(i = 16; i < 21; i++)
	{
			Tx_Buffer[i] = time[i-16];
	}
	
//	printf("-----Tx_Buffer in wtErrToFlash --------\n");
//	
//	for(i = 0; i < 10; i++)
//	{
//		printf("Tx_Buffer %d:  %x\n",i,Tx_Buffer[i]);
//	}
	
	
	sf_WriteBuffer(Tx_Buffer,FLASH_WriteAddress,SPI_FLASH_RWPageSize);
	
}







/*
***********************************************************************
*@brief    
					
*@param		num: exception number.
*@param		*code: exception number.
*@param		*ts:exception timestamp.
*@return  
**************************************************************************
*/
	
 
//void rdErrFromFlash(u16 num,u16 *code,char *ts)
//{
//	u16 errnum = 0;
//	int n = 0;
//	char *revbuf[3];
//	uint8_t Rx_Buffer[35]= " ";
//	
//	u32 FLASH_ReadAddress = (num%errcnt) * SPI_FLASH_PageSize;
//	
//	sf_ReadBuffer(Rx_Buffer,FLASH_ReadAddress,SPI_FLASH_RWPageSize);
//	//printf("In ReadExceptionFromFlash %s\n",Rx_Buffer);

//  split((char *)Rx_Buffer,",",revbuf,&n); //���ú������зָ� 
//   
//	errnum = atoi(revbuf[1]);
//	*code = errnum;
//	//printf("%s\n",revbuf[1]);
//	//printf("%s\n",revbuf[2]);
//		
//	sprintf(ts,"%s",revbuf[2]);		
//}



void RdTimeFromFlash(uint16_t num,uint8_t *rdBuffer)
{
	
	uint32_t FLASH_ReadAddress = (num%WRT_TIME_CNT)*SPI_FLASH_PageSize;
	
	sf_ReadBuffer(rdBuffer, FLASH_ReadAddress, SPI_FLASH_RWPageSize);

}





void rdErrFromFlash(u16 num,uint8_t Rx_Buffer[10])
{
	
	u32 FLASH_ReadAddress = (num%errcnt) * SPI_FLASH_PageSize;
	
	sf_ReadBuffer(Rx_Buffer,FLASH_ReadAddress,SPI_FLASH_RWPageSize);

}

void rdCmdFromFlash(u16 num,uint8_t *Rx_Buffer)
{
    u32 FLASH_ReadAddress = 0;
	
    if(num > CMD_CNT)
    {
        Rx_Buffer[0] = 0xFF;
        Rx_Buffer[1] = 0xFF;
        return;
    }
    
	FLASH_ReadAddress = num * SPI_FLASH_PageSize;
	
	sf_ReadBuffer(Rx_Buffer,FLASH_ReadAddress,SPI_FLASH_RWPageSize);

}






void Get_StoreCMD_Num(void)
{		
    uint16_t rdCMDAddr = 0;
    uint8_t Rd_CMDBuffer[50] = {0};
    uint16_t storeCMDNum = 0;
    for(rdCMDAddr = 0; rdCMDAddr < CMD_CNT; rdCMDAddr++)
    {
        rdCmdFromFlash(rdCMDAddr + 1, Rd_CMDBuffer);
        storeCMDNum = (Rd_CMDBuffer[0]<<8) + Rd_CMDBuffer[1];
        if(storeCMDNum == 0xFFFF)
        {
            break;
        }
    }

  
}


void Get_StoreException_Num(void)
{		
    uint16_t rdAddr = 0;
    uint8_t Rd_Buffer[50] = {0};
    uint16_t storeExcNum = 0;
    for(rdAddr=0;rdAddr < errcnt;rdAddr++)
    {
        rdErrFromFlash(rdAddr + 1,Rd_Buffer);
        storeExcNum = (Rd_Buffer[0]<<8) + Rd_Buffer[1];
        if(storeExcNum == 0xFFFF)
        {
            break;
        }
    }		
}
	

void Get_ExceptionCMD_Num(void)
{
    Get_StoreException_Num();

    Get_StoreCMD_Num();

}



void Empty_TimeLog(void)
{
	uint16_t i = 0;
	
	for(i = 0; i <= 2000;i++)
	{
		sf_EraseSector(i*SPI_FLASH_PageSize);	
	}			
	
}




void CalRunTime(void)
{

    g_tTotalRunTime.ucMinute += 5;
    
    if(g_tTotalRunTime.ucMinute >= 60)
    {
        g_tTotalRunTime.ucMinute %= 60;
        g_tTotalRunTime.ucHour += 1;
        
        if(g_tTotalRunTime.ucHour >= 24)
        {
            g_tTotalRunTime.ucHour %= 24;
            g_tTotalRunTime.ucDate+=1;
            
            if(g_tTotalRunTime.ucDate >= 30)
            {
                g_tTotalRunTime.ucDate %= 30;
                g_tTotalRunTime.ucMonth += 1;
                
                if(g_tTotalRunTime.ucMonth >= 12)
                {
                    g_tTotalRunTime.ucMonth %= 12;
                    
                    g_tTotalRunTime.ucYear += 1;
                }
            
            }
        }
    }

}


void BakTimeData(uint8_t *prerdbuf, uint8_t *rdbuff)
{
    uint8_t i = 0;

    for(i = 0; i < 5; i++)
    {
        prerdbuf[i] = rdbuff[i];
    
    }

}


void GetTotalRunTime(void)
{
    uint8_t i = 0;
    uint16_t usRdTimeAddr = 0;
    uint8_t Rd_TimeBuffer[10] = {0};
    uint8_t Pre_RdTimeBuffer[10] = {0};
    
    
    g_tTotalRunTime.ucYear   = 0;
    g_tTotalRunTime.ucMonth  = 0;
    g_tTotalRunTime.ucDate   = 0;
    g_tTotalRunTime.ucHour   = 0;
    g_tTotalRunTime.ucMinute = 0;    
    
    
    for(usRdTimeAddr = 0; usRdTimeAddr < WRT_TIME_CNT; usRdTimeAddr++)
    {
        RdTimeFromFlash(usRdTimeAddr, Rd_TimeBuffer);
                
        if((Rd_TimeBuffer[0] == 0xFF) || (Rd_TimeBuffer[1] == 0xFF) 
            || (Rd_TimeBuffer[2] == 0xFF) || (Rd_TimeBuffer[3] == 0xFF) || (Rd_TimeBuffer[4] == 0xFF))
        {
            break;
        }
              
        for(i = 0; i< 5; i++)
        {
            
            if(Rd_TimeBuffer[i] < Pre_RdTimeBuffer[i])
            {
                break;
            }
            
            if(Rd_TimeBuffer[i] > Pre_RdTimeBuffer[i])
            {
                g_ucWrtTimeCnt++;
                
                BakTimeData(Pre_RdTimeBuffer, Rd_TimeBuffer);
                
                g_tTotalRunTime.ucYear   = Rd_TimeBuffer[0];
                g_tTotalRunTime.ucMonth  = Rd_TimeBuffer[1];
                g_tTotalRunTime.ucDate   = Rd_TimeBuffer[2];    
                g_tTotalRunTime.ucHour   = Rd_TimeBuffer[3];    
                g_tTotalRunTime.ucMinute = Rd_TimeBuffer[4];
                
                Modbus[0x0090] = ((uint16_t)g_tTotalRunTime.ucYear << 8) + g_tTotalRunTime.ucMonth;
                Modbus[0x0091] = ((uint16_t)g_tTotalRunTime.ucDate << 8) + g_tTotalRunTime.ucHour;
                Modbus[0x0092] = (uint16_t)g_tTotalRunTime.ucMinute;                
                
                break;
            }           
        }
    
    } 
       
}



void SaveRunTime(void)
{
    uint8_t WrtTimeArry[6] = {0};
    
    uint32_t FLASH_WriteAddress = 0;    
    
    WrtTimeArry[0] = g_tTotalRunTime.ucYear;  
    WrtTimeArry[1] = g_tTotalRunTime.ucMonth; 
    WrtTimeArry[2] = g_tTotalRunTime.ucDate;  
    WrtTimeArry[3] = g_tTotalRunTime.ucHour;  
    WrtTimeArry[4] = g_tTotalRunTime.ucMinute;
    
    g_ucWrtTimeCnt = g_ucWrtTimeCnt%WRT_TIME_CNT;
    
 	FLASH_WriteAddress = g_ucWrtTimeCnt*SPI_FLASH_PageSize;                       	
	
    g_ucWrtTimeCnt++;

	sf_WriteBuffer(WrtTimeArry,FLASH_WriteAddress,SPI_FLASH_RWPageSize);   

}

