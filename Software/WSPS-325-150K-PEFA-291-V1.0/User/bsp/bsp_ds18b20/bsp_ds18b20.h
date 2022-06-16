/**
  **************************************************************************************
  * @file    bsp.h
  * @author  
  * @version V1.00
  * @date    2020-11-20
  * @brief   bsp header
  * @attention
  *
  * 
  * 
  * 
  *
  **************************************************************************************
  */  


#ifndef __BSP_DS18B20_H
#define	__BSP_DS18B20_H



#include "stm32l4xx.h"



/*Òý½Å¶¨Òå*/
/************************************************************/
                 
#define DS18B20_PIN                GPIO_PIN_10
#define DS18B20_PORT	           GPIOC
#define DS18B20_CLK_ENABLE()       __GPIOC_CLK_ENABLE()

/************************************************************/





#define DS18B20_HIGH  GPIO_PIN_SET
#define DS18B20_LOW   GPIO_PIN_RESET


#define Bit_RESET   0
#define Bit_SET     1



#define  DS18B20_DATA_IN()	          HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)
#define  DS18B20_DATA_OUT(value)	  HAL_GPIO_WritePin(DS18B20_PORT,DS18B20_PIN,value)




extern __IO uint16_t g_usBoardTemp;






uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);
void DS18B20_ReadId ( uint8_t * ds18b20_id );	



                    
                    
                               
void Get_Board_Temp(void);  




void DS18B20_GPIO_Config(void);





#endif

/*********************************************END OF FILE*************************************/
