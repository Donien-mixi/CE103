/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define PWM_LEFT_Pin GPIO_PIN_1
#define PWM_LEFT_GPIO_Port GPIOA
#define PWM_RIGHT_Pin GPIO_PIN_2
#define PWM_RIGHT_GPIO_Port GPIOA
#define Distance_0_Pin GPIO_PIN_12
#define Distance_0_GPIO_Port GPIOB
#define Distance_1_Pin GPIO_PIN_13
#define Distance_1_GPIO_Port GPIOB
#define Distance_2_Pin GPIO_PIN_14
#define Distance_2_GPIO_Port GPIOB
#define Distance_3_Pin GPIO_PIN_15
#define Distance_3_GPIO_Port GPIOB
#define Distance_4_Pin GPIO_PIN_8
#define Distance_4_GPIO_Port GPIOA
#define Distance_5_Pin GPIO_PIN_11
#define Distance_5_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_12
#define Buzzer_GPIO_Port GPIOA
#define LEFT_1_Pin GPIO_PIN_6
#define LEFT_1_GPIO_Port GPIOB
#define LEFT_2_Pin GPIO_PIN_7
#define LEFT_2_GPIO_Port GPIOB
#define RIGHT_1_Pin GPIO_PIN_8
#define RIGHT_1_GPIO_Port GPIOB
#define RIGHT_2_Pin GPIO_PIN_9
#define RIGHT_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
