#ifndef __CONTROL_H
#define __CONTROL_H


#include "stm32f4xx.h"


#define POWER_SW_PIN          GPIO_Pin_15                 
#define POWER_SW_PORT         GPIOC                      
#define POWER_SW_CLK          RCC_AHB1Periph_GPIOC

//#define POWER_ON()   GPIO_SetBits  (POWER_SW_PORT, POWER_SW_PIN)       // 高电平开 航星 雷波达诺
//#define POWER_OFF()  GPIO_ResetBits(POWER_SW_PORT, POWER_SW_PIN) 

#define POWER_ON()   GPIO_ResetBits(POWER_SW_PORT, POWER_SW_PIN)       // 低电平开 核达电源
#define POWER_OFF()  GPIO_SetBits  (POWER_SW_PORT, POWER_SW_PIN) 


#define RF_SW_PIN   GPIO_Pin_1              
#define RF_SW_PORT  GPIOA                      
#define RF_SW_CLK   RCC_AHB1Periph_GPIOA

#define RF_ON()     GPIO_SetBits  (RF_SW_PORT  , RF_SW_PIN)              
#define RF_OFF()    GPIO_ResetBits(RF_SW_PORT  , RF_SW_PIN) 

#define	BEER_ON()   GPIO_SetBits(GPIOC,GPIO_Pin_7)	
#define BEER_OFF()  GPIO_ResetBits(GPIOC,GPIO_Pin_7)	

//**********************ykj********************//
#define USERLOCK_EN()     GPIO_SetBits(GPIOE,GPIO_Pin_2);	
#define USERLOCK_DIS()    GPIO_ResetBits(GPIOE,GPIO_Pin_2);
#define USERLOCK()        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);	


void Source_OUT(void);
void Source_IN(void);
void Ctrl_GPIO_Init(void);
void Power_Control(void);
void Ptt_Control(void);
void TIMx_GPIO_Config(void);
void TIM_PWMOUTPUT_Config(unsigned int PulseWidth,unsigned char Duty_data);
void HMC1119_IO_Define(void);
void PWM_Form_CPU(void);
void PWM_Form_Outer(void);
void Atten_Control(u16 ATT);



void Power_Adj(void);


#endif

