
#ifndef __RFE001_H 
#define __RFE001_H 	 
#include "stm32f4xx.h"

#include "tmNxTypes.h"
#include "Volcano.h"
#include "Volcano_local.h"





/**************************REF0001_IO INIT*******************/

#define  RCC_GPIO_REF001_NSS1      RCC_AHB1Periph_GPIOD
#define  GPIO_REF001_NSS1          GPIOD
#define  PIN_REF0001_NSS1          GPIO_Pin_0
#define  PIN_REF0001_NSS1_ON()     GPIO_SetBits  (GPIO_REF001_NSS1, PIN_REF0001_NSS1) 
#define  PIN_REF0001_NSS1_OFF()    GPIO_ResetBits(GPIO_REF001_NSS1, PIN_REF0001_NSS1) 
                                   
#define  RCC_GPIO_REF001_SCK       RCC_AHB1Periph_GPIOC                         
#define  GPIO_REF001_SCK           GPIOC
#define  PIN_REF0001_SCK           GPIO_Pin_10
#define  PIN_REF0001_SCK_ON()      GPIO_SetBits  (GPIO_REF001_SCK, PIN_REF0001_SCK) 
#define  PIN_REF0001_SCK_OFF()     GPIO_ResetBits(GPIO_REF001_SCK, PIN_REF0001_SCK) 
                                   
#define  RCC_GPIO_REF001_MOSI      RCC_AHB1Periph_GPIOC                              
#define  GPIO_REF001_MOSI          GPIOC
#define  PIN_REF0001_MOSI          GPIO_Pin_12
#define  PIN_REF0001_MOSI_ON()     GPIO_SetBits  (GPIO_REF001_MOSI, PIN_REF0001_MOSI) 
#define  PIN_REF0001_MOSI_OFF()    GPIO_ResetBits(GPIO_REF001_MOSI, PIN_REF0001_MOSI) 
                                   
#define  RCC_GPIO_REF001_MISO      RCC_AHB1Periph_GPIOC                              
#define  GPIO_REF001_MISO          GPIOC
#define  PIN_REF0001_MISO          GPIO_Pin_11
#define  PIN_REF0001_MISO_IN()     GPIO_ReadInputDataBit(GPIO_REF001_MISO,PIN_REF0001_MISO)
                                   
//#define  RCC_GPIO_REF001_SW        RCC_AHB1Periph_GPIOD                               
//#define  GPIO_REF001_SW            GPIOB
//#define  PIN_REF0001_SW            GPIO_Pin_9
//#define  PIN_REF0001_SW_ON()       GPIO_SetBits  (GPIO_REF001_SW, PIN_REF0001_SW)
//#define  PIN_REF0001_SW_OFF()      GPIO_ResetBits(GPIO_REF001_SW, PIN_REF0001_SW)

#define  RCC_GPIO_REF001_NSS2      RCC_AHB1Periph_GPIOD                            
#define  GPIO_REF001_NSS2          GPIOD                                           
#define  PIN_REF0001_NSS2          GPIO_Pin_4                                     
#define  PIN_REF0001_NSS2_ON()     GPIO_SetBits  (GPIO_REF001_NSS2, PIN_REF0001_NSS2)
#define  PIN_REF0001_NSS2_OFF()    GPIO_ResetBits(GPIO_REF001_NSS2, PIN_REF0001_NSS2)

void REF0001_Plus_OUT(unsigned int REF0001_Plus_Width,unsigned int REF0001_Plus_Duty);
void REF0001_ATT_OUT(unsigned int POWER_DATA_dBM);
void REF0001_FERQ_OUT(unsigned int FREQ_DATA_MHZ);
void REF0001_INIT(unsigned int FREQ_DATA_MHZ,unsigned int POWER_DATA_dBM);
void RFE001_IO_INIT(void);
void REF0001_Phase_OUT(unsigned int REF0001_Phase_DU);
tmErrorCode_t UserWrittenSPIWrite (tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pData);
tmErrorCode_t UserWrittenSPIWriteRead(tmUnitSelect_t tUnit, UInt32 WriteBitsLen, UInt8* pDataWrite, UInt32 ReadBitsLen, UInt8* pDataRead);
tmErrorCode_t UserWrittenWait(UInt32 tms);

#endif 


