#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32h7xx.h"
#include <stdio.h>

#define errcnt 2000

#define SPI_FLASH_PageSize 4096
#define SPI_FLASH_TotalSize 8 * 1024 * 1024
#define SPI_FLASH_PerWritePageSize 256 //页字节大小
#define SPI_FLASH_RWPageSize 50        //页字节大小
#define SPI_FLASH_Store_ErrNum_Addr 0x0000

#define EXCEPT_LOG_STARADDR 0x1000
#define EXCEPT_NEXTDIS_ADDR_GAP 0x0E
#define EXCEPT_DIS_ADDR_GAP 0x02
#define errPageNumMax 0xC8

#define FLASHReadWriteSize 4096
#define NumByteToReadWrite 23

extern __IO uint8_t errPageNum;
extern __IO uint8_t excFlag;
extern __IO uint16_t errRdNum;
extern __IO uint16_t errWtNum;

void Get_StoreException_Num(void);
void wtErrToFlash(uint16_t num, uint16_t code, __IO uint8_t *time_arry);
void rdErrFromFlash(uint16_t num, uint8_t Rx_Buffer[10]);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_ReadBuffer(uint8_t *_pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);

void Store_Exception_To_Flash(uint16_t code);

#endif /* __SPI_FLASH_H */
