/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Machine_LED3_Pin GPIO_PIN_2
#define Machine_LED3_GPIO_Port GPIOE
#define Machine_LED4_Pin GPIO_PIN_3
#define Machine_LED4_GPIO_Port GPIOE
#define Machine_LED5_Pin GPIO_PIN_4
#define Machine_LED5_GPIO_Port GPIOE
#define Machine_LED6_Pin GPIO_PIN_5
#define Machine_LED6_GPIO_Port GPIOE
#define P_D0_Pin GPIO_PIN_0
#define P_D0_GPIO_Port GPIOG
#define P_D1_Pin GPIO_PIN_1
#define P_D1_GPIO_Port GPIOG
#define W5500_RST1_Pin GPIO_PIN_8
#define W5500_RST1_GPIO_Port GPIOE
#define W5500_INIT1_Pin GPIO_PIN_9
#define W5500_INIT1_GPIO_Port GPIOE
#define SPI4_NSS_Pin GPIO_PIN_11
#define SPI4_NSS_GPIO_Port GPIOE
#define LINKLED1_Pin GPIO_PIN_15
#define LINKLED1_GPIO_Port GPIOE
#define W5500_RST2_Pin GPIO_PIN_10
#define W5500_RST2_GPIO_Port GPIOB
#define W5500_INIT2_Pin GPIO_PIN_11
#define W5500_INIT2_GPIO_Port GPIOB
#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define LINKLED2_Pin GPIO_PIN_10
#define LINKLED2_GPIO_Port GPIOD
#define P_D2_Pin GPIO_PIN_2
#define P_D2_GPIO_Port GPIOG
#define P_D3_Pin GPIO_PIN_3
#define P_D3_GPIO_Port GPIOG
#define P_D4_Pin GPIO_PIN_4
#define P_D4_GPIO_Port GPIOG
#define P_D5_Pin GPIO_PIN_5
#define P_D5_GPIO_Port GPIOG
#define P_D6_Pin GPIO_PIN_6
#define P_D6_GPIO_Port GPIOG
#define P_D7_Pin GPIO_PIN_7
#define P_D7_GPIO_Port GPIOG
#define P_D8_Pin GPIO_PIN_8
#define P_D8_GPIO_Port GPIOG
#define Power_SW_Pin GPIO_PIN_0
#define Power_SW_GPIO_Port GPIOD
#define P_D9_Pin GPIO_PIN_9
#define P_D9_GPIO_Port GPIOG
#define P_D10_Pin GPIO_PIN_10
#define P_D10_GPIO_Port GPIOG
#define P_D11_Pin GPIO_PIN_11
#define P_D11_GPIO_Port GPIOG
#define P_D12_Pin GPIO_PIN_12
#define P_D12_GPIO_Port GPIOG
#define P_D13_Pin GPIO_PIN_13
#define P_D13_GPIO_Port GPIOG
#define P_D14_Pin GPIO_PIN_14
#define P_D14_GPIO_Port GPIOG
#define P_D15_Pin GPIO_PIN_15
#define P_D15_GPIO_Port GPIOG
#define S_CLK_Pin GPIO_PIN_8
#define S_CLK_GPIO_Port GPIOB
#define Machine_LED1_Pin GPIO_PIN_0
#define Machine_LED1_GPIO_Port GPIOE
#define Machine_LED2_Pin GPIO_PIN_1
#define Machine_LED2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
