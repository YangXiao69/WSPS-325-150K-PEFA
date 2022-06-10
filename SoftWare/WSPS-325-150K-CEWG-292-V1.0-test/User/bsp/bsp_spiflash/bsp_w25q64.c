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
 
  //设置引脚复用
  GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,FLASH_SPI_SCK_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,FLASH_SPI_MISO_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,FLASH_SPI_MOSI_AF); 
  
  /*!< 配置 SPI_FLASH_SPI 引脚: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引脚: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引脚: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< 配置 SPI_FLASH_SPI 引脚: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);

  /* 停止信号 FLASH: CS引脚高电平*/
  SPI_FLASH_CS_HIGH();

  /* FLASH_SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
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

  /* 使能 FLASH_SPI  */
  SPI_Cmd(FLASH_SPI, ENABLE);

}

 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  SPI_FLASH_WriteEnable();                 //发送FLASH写使能命令 
  SPI_FLASH_WaitForWriteEnd();             //等待命令完成

  SPI_FLASH_CS_LOW();                      //片选
  SPI_FLASH_SendByte(W25X_SectorErase);    //发送扇区擦除指令

  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);  //发送擦除扇区地址的高位
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);     //发送擦除扇区地址的中位
  SPI_FLASH_SendByte(SectorAddr & 0xFF);              //发送擦除扇区地址的低位

  SPI_FLASH_CS_HIGH();               //停止信号 FLASH: CS 高电平
  SPI_FLASH_WaitForWriteEnd();       //等待擦除完毕
}


 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
  
  SPI_FLASH_WriteEnable();   //发送FLASH写使能命令 
  SPI_FLASH_CS_LOW();

  SPI_FLASH_SendByte(W25X_ChipErase);  /* 发送整块擦除指令*/
 
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  
  SPI_FLASH_CS_LOW();                                /* 选择FLASH: CS低电平 */
  SPI_FLASH_SendByte(W25X_PageProgram);              /* 写页写指令*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);  /*发送写地址的高位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);     /*发送写地址的中位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);              /*发送写地址的低位*/

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;	
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  
  SPI_FLASH_CS_LOW();/* 选择FLASH: CS低电平 */

  SPI_FLASH_SendByte(W25X_ReadData);                /* 发送 读 指令 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);  /* 发送 读 地址高位 */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);      /* 发送 读 地址中位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);              /* 发送 读 地址低位 */

  while (NumByteToRead--)/* 读取数据 */
  {
    /* 读取一个字节*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  SPI_FLASH_CS_LOW();                           /* 开始通讯：CS低电平 */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);       /* 发送JEDEC指令，读取ID */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);       /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);       /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);       /* 读取一个字节数据 */
  SPI_FLASH_CS_HIGH();                          /* 停止通讯：CS高电平 */
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;  /*把数据组合起来，作为函数的返回值*/
  return Temp;
}

 /**
  * @brief  读取FLASH Device ID
  * @param 	无
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
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(FLASH_SPI, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
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
  * @brief  向FLASH发送 写使能 命令
  */
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();                   /* 通讯开始：CS低 */
  SPI_FLASH_SendByte(W25X_WriteEnable); /* 发送写使能命令*/
  SPI_FLASH_CS_HIGH();                  /*通讯结束：CS高 */
}

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();                     /* 选择 FLASH: CS 低 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg); /* 发送 读状态寄存器 命令 */
  SPITimeout = SPIT_FLAG_TIMEOUT;
  do                                      /* 若FLASH忙碌，则等待 */
  {
    
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 /* 读取FLASH芯片的状态寄存器 */

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */
  SPI_FLASH_CS_HIGH();/* 停止信号  FLASH: CS 高 */
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  SPI_FLASH_CS_LOW();/* 选择 FLASH: CS 低 */
  SPI_FLASH_SendByte(W25X_PowerDown);/* 发送 掉电 命令 */
  SPI_FLASH_CS_HIGH();/* 停止信号  FLASH: CS 高 */
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  SPI_FLASH_CS_LOW();/*选择 FLASH: CS 低 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);/* 发上 上电 命令 */
  SPI_FLASH_CS_HIGH();                   //等待TRES1
}   


/*
  * @brief  等待超时回调函数
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  return 0;
}
   





/*
        src 源字符串的首地址(buf的地址) 
        separator 指定的分割字符
        dest 接收子字符串的数组
        num 分割后子字符串的个数
*/



void split(char *src, const char *separator, char **dest, int *num) 
{

     char *pNext;
     int count = 0;
     
     if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止 
        return;
        
     if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止 
        return;
        
     pNext = (char *)strtok(src,separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
    }  
    *num = count;
}     


uint8_t bsp_spiRead1(void)
{
	/* 硬件SPI */
	uint8_t read;

	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPI5, 0);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	read = SPI_I2S_ReceiveData(SPI5);

	/* 返回读到的数据 */
	return read;

}

void bsp_spiWrite1(uint8_t _ucByte)
{

	/* 硬件SPI */
	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPI5, _ucByte);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	SPI_I2S_ReceiveData(SPI5);

}




static void sf_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  
  SPI_FLASH_CS_LOW();                     /* 选择 FLASH: CS 低 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg); /* 发送 读状态寄存器 命令 */
  SPITimeout = SPIT_FLAG_TIMEOUT;
  do                                      /* 若FLASH忙碌，则等待 */
  {
    
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 /* 读取FLASH芯片的状态寄存器 */

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */
  SPI_FLASH_CS_HIGH();/* 停止信号  FLASH: CS 高 */
}


static void sf_WriteEnable(void)
{
    SPI_FLASH_CS_LOW();                   /* 通讯开始：CS低 */
    SPI_FLASH_SendByte(W25X_WriteEnable); /* 发送写使能命令*/
    SPI_FLASH_CS_HIGH();                  /*通讯结束：CS高 */	
}


void sf_EraseSector(uint32_t _uiSectorAddr)
{
  SPI_FLASH_WriteEnable();                 //发送FLASH写使能命令 
  SPI_FLASH_WaitForWriteEnd();             //等待命令完成

  SPI_FLASH_CS_LOW();                      //片选
  SPI_FLASH_SendByte(W25X_SectorErase);    //发送扇区擦除指令

  SPI_FLASH_SendByte((_uiSectorAddr & 0xFF0000) >> 16);  //发送擦除扇区地址的高位
  SPI_FLASH_SendByte((_uiSectorAddr & 0xFF00) >> 8);     //发送擦除扇区地址的中位
  SPI_FLASH_SendByte(_uiSectorAddr & 0xFF);              //发送擦除扇区地址的低位

  SPI_FLASH_CS_HIGH();               //停止信号 FLASH: CS 高电平
  SPI_FLASH_WaitForWriteEnd();       //等待擦除完毕
}


void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{
	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > SPI_FLASH_TotalSize)
	{
		return;
	}

	/* 擦除扇区操作 */
	SPI_FLASH_CS_LOW();									/* 使能片选 */
	bsp_spiWrite1(W25X_ReadData);							/* 发送读命令 */
	bsp_spiWrite1((_uiReadAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
	bsp_spiWrite1((_uiReadAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
	bsp_spiWrite1(_uiReadAddr & 0xFF);				/* 发送扇区地址低8bit */
	while (_uiSize--)
	{
		*_pBuf++ = bsp_spiRead1();			/* 读一个字节并存储到pBuf，读完后指针自加1 */
	}
	SPI_FLASH_CS_HIGH();									/* 禁能片选 */
}


static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;


	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* 注意错误的写法: if (ucOld & (*_ucpNewBuf++) != 0) */
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
        sf_WriteEnable();								/* 发送写使能命令 */
    
        SPI_FLASH_CS_LOW();									/* 使能片选 */
        bsp_spiWrite1(0x02);								/* 发送AAI命令(地址自动增加编程) */
        bsp_spiWrite1((_uiWriteAddr & 0xFF0000) >> 16);	/* 发送扇区地址的高8bit */
        bsp_spiWrite1((_uiWriteAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
        bsp_spiWrite1(_uiWriteAddr & 0xFF);				/* 发送扇区地址低8bit */
    
        for (i = 0; i < 256; i++)
        {
            bsp_spiWrite1(*_pBuf++);					/* 发送数据 */
        }
    
        SPI_FLASH_CS_HIGH();								/* 禁止片选 */
    
        sf_WaitForWriteEnd();						/* 等待串行Flash内部写操作完成 */
    
        _uiWriteAddr += 256;
    }
    
    /* 进入写保护状态 */
    SPI_FLASH_CS_LOW();
    bsp_spiWrite1(W25X_WriteDisable);
    SPI_FLASH_CS_HIGH();
    
    sf_WaitForWriteEnd();							/* 等待串行Flash内部写操作完成 */
	
}


static uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint8_t ucValue;

	/* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
	if ((_uiSrcAddr + _uiSize) > SPI_FLASH_TotalSize)
	{
		return 1;
	}

	if (_uiSize == 0)
	{
		return 0;
	}

	SPI_FLASH_CS_LOW();									/* 使能片选 */
	bsp_spiWrite1(W25X_ReadData);							/* 发送读命令 */
	bsp_spiWrite1((_uiSrcAddr & 0xFF0000) >> 16);		/* 发送扇区地址的高8bit */
	bsp_spiWrite1((_uiSrcAddr & 0xFF00) >> 8);		/* 发送扇区地址中间8bit */
	bsp_spiWrite1(_uiSrcAddr & 0xFF);					/* 发送扇区地址低8bit */
	while (_uiSize--)
	{
		/* 读一个字节 */
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
	uint16_t j;					/* 用于延时 */
	uint32_t uiFirstAddr;		/* 扇区首址 */
	uint8_t ucNeedErase;		/* 1表示需要擦除 */
	uint8_t cRet;

	/* 长度为0时不继续操作,直接认为成功 */
	if (_usWrLen == 0)
	{
		return 1;
	}

	/* 如果偏移地址超过芯片容量则退出 */
	if (_uiWrAddr >= SPI_FLASH_TotalSize)
	{
		return 0;
	}

	/* 如果数据长度大于扇区容量，则退出 */
	if (_usWrLen > SPI_FLASH_PageSize)
	{
		return 0;
	}

	/* 如果FLASH中的数据没有变化,则不写FLASH */
	sf_ReadBuffer(s_spiBuf, _uiWrAddr, _usWrLen);
	if (memcmp(s_spiBuf, _ucpSrc, _usWrLen) == 0)
	{
		return 1;
	}

	/* 判断是否需要先擦除扇区 */
	/* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
	ucNeedErase = 0;
	if (sf_NeedErase(s_spiBuf, _ucpSrc, _usWrLen))
	{
		ucNeedErase = 1;
	}

	uiFirstAddr = _uiWrAddr & (~(SPI_FLASH_PageSize - 1));

	if (_usWrLen == SPI_FLASH_PageSize)		/* 整个扇区都改写 */
	{
		for	(i = 0; i < SPI_FLASH_PageSize; i++)
		{
			s_spiBuf[i] = _ucpSrc[i];
		}
	}
	else						/* 改写部分数据 */
	{
		/* 先将整个扇区的数据读出 */
		sf_ReadBuffer(s_spiBuf, uiFirstAddr, SPI_FLASH_PageSize);

		/* 再用新数据覆盖 */
		i = _uiWrAddr & (SPI_FLASH_PageSize - 1);
		memcpy(&s_spiBuf[i], _ucpSrc, _usWrLen);
	}

	/* 写完之后进行校验，如果不正确则重写，最多3次 */
	cRet = 0;
	for (i = 0; i < 3; i++)
	{

		/* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
		if (ucNeedErase == 1)
		{
			sf_EraseSector(uiFirstAddr);		/* 擦除1个扇区 */
		}

		/* 编程一个PAGE */
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

			/* 失败后延迟一段时间再重试 */
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

	if (Addr == 0) /* 起始地址是页面首地址  */
	{
		if (NumOfPage == 0) /* 数据长度小于页面大小 */
		{
			if (sf_AutoWritePage(_pBuf, _uiWriteAddr, _usWriteSize) == 0)
			{
				return 0;
			}
		}
		else 	/* 数据长度大于等于页面大小 */
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
	else  /* 起始地址不是页面首地址  */
	{
		if (NumOfPage == 0) /* 数据长度小于页面大小 */
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
		else	/* 数据长度大于等于页面大小 */
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
	return 1;	/* 成功 */
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

//  split((char *)Rx_Buffer,",",revbuf,&n); //调用函数进行分割 
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

