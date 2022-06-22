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
#include "stm32f1xx_hal.h"

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
#define AD2_D2_Pin GPIO_PIN_2
#define AD2_D2_GPIO_Port GPIOE
#define AD2_D3_Pin GPIO_PIN_3
#define AD2_D3_GPIO_Port GPIOE
#define AD2_D4_Pin GPIO_PIN_4
#define AD2_D4_GPIO_Port GPIOE
#define AD2_D5_Pin GPIO_PIN_5
#define AD2_D5_GPIO_Port GPIOE
#define AD2_D6_Pin GPIO_PIN_6
#define AD2_D6_GPIO_Port GPIOE
#define P_D13_Pin GPIO_PIN_13
#define P_D13_GPIO_Port GPIOC
#define P_D14_Pin GPIO_PIN_14
#define P_D14_GPIO_Port GPIOC
#define P_D15_Pin GPIO_PIN_15
#define P_D15_GPIO_Port GPIOC
#define P_D0_Pin GPIO_PIN_0
#define P_D0_GPIO_Port GPIOC
#define P_D1_Pin GPIO_PIN_1
#define P_D1_GPIO_Port GPIOC
#define P_D2_Pin GPIO_PIN_2
#define P_D2_GPIO_Port GPIOC
#define P_D3_Pin GPIO_PIN_3
#define P_D3_GPIO_Port GPIOC
#define AD2_CS_Pin GPIO_PIN_2
#define AD2_CS_GPIO_Port GPIOA
#define AD2_RD_Pin GPIO_PIN_3
#define AD2_RD_GPIO_Port GPIOA
#define AD2_CONVST_Pin GPIO_PIN_4
#define AD2_CONVST_GPIO_Port GPIOA
#define AD2_BYTE_Pin GPIO_PIN_5
#define AD2_BYTE_GPIO_Port GPIOA
#define AD2_RESET_Pin GPIO_PIN_6
#define AD2_RESET_GPIO_Port GPIOA
#define AD2_BUSY_Pin GPIO_PIN_7
#define AD2_BUSY_GPIO_Port GPIOA
#define P_D4_Pin GPIO_PIN_4
#define P_D4_GPIO_Port GPIOC
#define P_D5_Pin GPIO_PIN_5
#define P_D5_GPIO_Port GPIOC
#define AD2_D7_Pin GPIO_PIN_7
#define AD2_D7_GPIO_Port GPIOE
#define AD2_D8_Pin GPIO_PIN_8
#define AD2_D8_GPIO_Port GPIOE
#define AD2_D9_Pin GPIO_PIN_9
#define AD2_D9_GPIO_Port GPIOE
#define AD2_D10_Pin GPIO_PIN_10
#define AD2_D10_GPIO_Port GPIOE
#define AD2_D11_Pin GPIO_PIN_11
#define AD2_D11_GPIO_Port GPIOE
#define AD2_D12_Pin GPIO_PIN_12
#define AD2_D12_GPIO_Port GPIOE
#define AD2_D13_Pin GPIO_PIN_13
#define AD2_D13_GPIO_Port GPIOE
#define AD2_D14_Pin GPIO_PIN_14
#define AD2_D14_GPIO_Port GPIOE
#define AD2_D15_Pin GPIO_PIN_15
#define AD2_D15_GPIO_Port GPIOE
#define AD1_CS_Pin GPIO_PIN_10
#define AD1_CS_GPIO_Port GPIOB
#define AD1_RD_Pin GPIO_PIN_11
#define AD1_RD_GPIO_Port GPIOB
#define AD1_CONVST_Pin GPIO_PIN_12
#define AD1_CONVST_GPIO_Port GPIOB
#define AD1_BYTE_Pin GPIO_PIN_13
#define AD1_BYTE_GPIO_Port GPIOB
#define AD1_RESET_Pin GPIO_PIN_14
#define AD1_RESET_GPIO_Port GPIOB
#define AD1_BUSY_Pin GPIO_PIN_15
#define AD1_BUSY_GPIO_Port GPIOB
#define AD1_D8_Pin GPIO_PIN_8
#define AD1_D8_GPIO_Port GPIOD
#define AD1_D9_Pin GPIO_PIN_9
#define AD1_D9_GPIO_Port GPIOD
#define AD1_D10_Pin GPIO_PIN_10
#define AD1_D10_GPIO_Port GPIOD
#define AD1_D11_Pin GPIO_PIN_11
#define AD1_D11_GPIO_Port GPIOD
#define AD1_D12_Pin GPIO_PIN_12
#define AD1_D12_GPIO_Port GPIOD
#define AD1_D13_Pin GPIO_PIN_13
#define AD1_D13_GPIO_Port GPIOD
#define AD1_D14_Pin GPIO_PIN_14
#define AD1_D14_GPIO_Port GPIOD
#define AD1_D15_Pin GPIO_PIN_15
#define AD1_D15_GPIO_Port GPIOD
#define P_D6_Pin GPIO_PIN_6
#define P_D6_GPIO_Port GPIOC
#define P_D7_Pin GPIO_PIN_7
#define P_D7_GPIO_Port GPIOC
#define P_D8_Pin GPIO_PIN_8
#define P_D8_GPIO_Port GPIOC
#define P_D9_Pin GPIO_PIN_9
#define P_D9_GPIO_Port GPIOC
#define P_D10_Pin GPIO_PIN_10
#define P_D10_GPIO_Port GPIOC
#define P_D11_Pin GPIO_PIN_11
#define P_D11_GPIO_Port GPIOC
#define P_D12_Pin GPIO_PIN_12
#define P_D12_GPIO_Port GPIOC
#define AD1_D0_Pin GPIO_PIN_0
#define AD1_D0_GPIO_Port GPIOD
#define AD1_D1_Pin GPIO_PIN_1
#define AD1_D1_GPIO_Port GPIOD
#define AD1_D2_Pin GPIO_PIN_2
#define AD1_D2_GPIO_Port GPIOD
#define AD1_D3_Pin GPIO_PIN_3
#define AD1_D3_GPIO_Port GPIOD
#define AD1_D4_Pin GPIO_PIN_4
#define AD1_D4_GPIO_Port GPIOD
#define AD1_D5_Pin GPIO_PIN_5
#define AD1_D5_GPIO_Port GPIOD
#define AD1_D6_Pin GPIO_PIN_6
#define AD1_D6_GPIO_Port GPIOD
#define AD1_D7_Pin GPIO_PIN_7
#define AD1_D7_GPIO_Port GPIOD
#define AD2_D0_Pin GPIO_PIN_0
#define AD2_D0_GPIO_Port GPIOE
#define AD2_D1_Pin GPIO_PIN_1
#define AD2_D1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
