#include "./../../system.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

__IO uint8_t errPageNum = 0;
__IO uint8_t excFlag = 0;
__IO uint16_t errRdNum = 0;
__IO uint16_t errWtNum = 0;

/*
***********************************************************************
*@brief

*@param		num: exception number.
*@param		*code: exception number.
*@param		*ts:	exception timestamp.
*@return
**************************************************************************
*/
// void wtErrToFlash(uint16_t num,uint16_t code,uint8_t time[5])
//{
//	char numstr[10];
//	char codestr[10] = "";
//	char Tx_Buffer[50] = "";
//
//	uint32_t FLASH_WriteAddress = (num%errcnt) * SPI_FLASH_PageSize;
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
//	sf_WriteBuffer((uint8_t*)Tx_Buffer,FLASH_WriteAddress,SPI_FLASH_RWPageSize);
//
//}

static int strToHex(char *str)
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
        else
        {
            return 0;
        }
        str++;
    }

    return result;
}

void wtErrToFlash(uint16_t num, uint16_t code, __IO uint8_t *time_arry)
{
    uint8_t i = 0;
    uint16_t m = 0, n = 0;
    uint8_t Tx_Buffer[10] = {0};
    char numstr[5] = "";
    char codestr[5] = "";

    uint32_t FLASH_WriteAddress = (num % errcnt) * SPI_FLASH_PageSize;

    sprintf(numstr, "%x", num);
    sprintf(codestr, "%x", code);

    //	printf("numstr %s \n",numstr);
    //	printf("codestr %s \n",codestr);

    m = strToHex(numstr);
    n = strToHex(codestr);

    //	printf("n: %x",n);

    Tx_Buffer[0] = (m >> 8) & 0xFF;
    Tx_Buffer[1] = m & 0xFF;

    Tx_Buffer[2] = (n >> 8) & 0xFF;
    Tx_Buffer[3] = n & 0xFF;

    for (i = 4; i < 10; i++)
    {
        Tx_Buffer[i] = time_arry[i - 4];
    }

    //	printf("-----Tx_Buffer in wtErrToFlash --------\n");
    //
    //	for(i = 0; i < 10; i++)
    //	{
    //		printf("Tx_Buffer %d:  %x\n",i,Tx_Buffer[i]);
    //	}

    sf_WriteBuffer(Tx_Buffer, FLASH_WriteAddress, SPI_FLASH_RWPageSize);
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

// void rdErrFromFlash(uint16_t num,uint16_t *code,char *ts)
//{
//	uint16_t errnum = 0;
//	int n = 0;
//	char *revbuf[3];
//	uint8_t Rx_Buffer[35]= " ";
//
//	uint32_t FLASH_ReadAddress = (num%errcnt) * SPI_FLASH_PageSize;
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

void rdErrFromFlash(uint16_t num, uint8_t Rx_Buffer[10])
{

    uint32_t FLASH_ReadAddress = (num % errcnt) * SPI_FLASH_PageSize;

    sf_ReadBuffer(Rx_Buffer, FLASH_ReadAddress, SPI_FLASH_RWPageSize);
}

void Get_StoreException_Num(void)
{
    uint16_t rdAddr = 0;
    uint8_t Rd_Buffer[50] = {0};
    uint16_t storeExcNum = 0;
    for (rdAddr = 0; rdAddr < 2000; rdAddr++)
    {
        rdErrFromFlash(rdAddr + 1, Rd_Buffer);
        storeExcNum = (Rd_Buffer[0] << 8) + Rd_Buffer[1];
        if (storeExcNum == 0xFFFF)
        {
            break;
        }
        errWtNum = storeExcNum;
    }
}

void Store_Exception_To_Flash(uint16_t code)
{
    uint16_t num = 0;

    errWtNum = errWtNum % errcnt;
    num = ++errWtNum;

    Get_Time();
    delay_ms(50);
    delay_ms(50);
    wtErrToFlash(num, code, SystemTime);
}
