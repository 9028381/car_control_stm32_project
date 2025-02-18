/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define CCD_SI_Pin GPIO_PIN_6
#define CCD_SI_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define IO1_Pin GPIO_PIN_9
#define IO1_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOC
#define BUZZ_Pin GPIO_PIN_1
#define BUZZ_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define CCD_CLK_Pin GPIO_PIN_3
#define CCD_CLK_GPIO_Port GPIOC
#define IO4_Pin GPIO_PIN_4
#define IO4_GPIO_Port GPIOA
#define IO9_Pin GPIO_PIN_1
#define IO9_GPIO_Port GPIOB
#define IO8_Pin GPIO_PIN_2
#define IO8_GPIO_Port GPIOB
#define IO7_Pin GPIO_PIN_7
#define IO7_GPIO_Port GPIOE
#define IO6_Pin GPIO_PIN_8
#define IO6_GPIO_Port GPIOE
#define IO5_Pin GPIO_PIN_10
#define IO5_GPIO_Port GPIOE
#define M1D2_Pin GPIO_PIN_10
#define M1D2_GPIO_Port GPIOA
#define M2D1_Pin GPIO_PIN_11
#define M2D1_GPIO_Port GPIOA
#define M2D2_Pin GPIO_PIN_12
#define M2D2_GPIO_Port GPIOA
#define M1D1_Pin GPIO_PIN_12
#define M1D1_GPIO_Port GPIOC
#define CS0_Pin GPIO_PIN_1
#define CS0_GPIO_Port GPIOD
#define M3D1_Pin GPIO_PIN_5
#define M3D1_GPIO_Port GPIOD
#define M3D2_Pin GPIO_PIN_6
#define M3D2_GPIO_Port GPIOD
#define M4D1_Pin GPIO_PIN_7
#define M4D1_GPIO_Port GPIOD
#define M4D2_Pin GPIO_PIN_3
#define M4D2_GPIO_Port GPIOB
#define IO3_Pin GPIO_PIN_0
#define IO3_GPIO_Port GPIOE
#define IO2_Pin GPIO_PIN_1
#define IO2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
