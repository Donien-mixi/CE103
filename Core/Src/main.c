/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TCRT5000.h"
#include "Motor.h"
#include "E3F.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    STATE_SURVIVAL,
    STATE_ATTACK,
    STATE_DEFENSE,
    STATE_SEARCH,
    STATE_WAITING
} RobotState_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
Motor_t motorL, motorR;
LineArray_t LineArray;

E3FArray_t DistanceArray;
GPIO_TypeDef* e3f_ports[E3F_COUNT] = {GPIOB, GPIOB, GPIOB, GPIOB, GPIOA, GPIOA};
uint16_t e3f_pins[E3F_COUNT] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_8, GPIO_PIN_11};

RobotState_t CurrentState = STATE_WAITING;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Survival_Mode(void);
void Attack_Mode(void);
void Defense_Mode(void);
void Search_Mode(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_TIM2_Init();

    /* USER CODE BEGIN 2 */
    Line_Init(&LineArray, true);
    E3FArray_Init(&DistanceArray, e3f_ports, e3f_pins, GPIO_PIN_SET);

    Motor_Init(&motorL, &htim2, TIM_CHANNEL_2, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);
    Motor_Init(&motorR, &htim2, TIM_CHANNEL_3, GPIOB, GPIO_PIN_8, GPIOB, GPIO_PIN_9);

    // LUẬT 5 GIÂY VÀ KHỞI ĐỘNG CHIẾN THUẬT
    Motor_SetSpeed(&motorL, 0);
    Motor_SetSpeed(&motorR, 0);
    HAL_Delay(4500);

    // Chiến thuật lao ra giữa sân ban đầu
    Motor_SetSpeed(&motorL, -1000);
    Motor_SetSpeed(&motorR, 1000);
    HAL_Delay(3200);

    static uint32_t last_active_time = 0;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        // 1. CẬP NHẬT DỮ LIỆU CẢM BIẾN
        Line_Update(&LineArray);
        E3FArray_Update(&DistanceArray);

        uint8_t force_web_update = 0;

        // 2. QUYẾT ĐỊNH TRẠNG THÁI VÀ ĐIỀU KHIỂN ĐỘNG CƠ

        // ƯU TIÊN 1: SINH TỒN
        if (!Line_NoDetection(&LineArray)) {
            if (CurrentState != STATE_SURVIVAL) {
                CurrentState = STATE_SURVIVAL;
                force_web_update = 1;
            }
            Survival_Mode();
        }
        // ƯU TIÊN 2: TẤN CÔNG
        else if (DistanceArray.Results[1] || DistanceArray.Results[2] || DistanceArray.Results[3]) {
            if (CurrentState != STATE_ATTACK) {
                CurrentState = STATE_ATTACK;
                force_web_update = 1;
            }
            Attack_Mode();
        }
        // ƯU TIÊN 3: PHÒNG THỦ
        else if (DistanceArray.Results[0] || DistanceArray.Results[4] || DistanceArray.Results[5]) {
            if (CurrentState != STATE_DEFENSE) {
                CurrentState = STATE_DEFENSE;
                force_web_update = 1;
            }
            Defense_Mode();
        }
        // ƯU TIÊN 4: TÌM KIẾM
        else {
            if (CurrentState != STATE_SEARCH) {
                CurrentState = STATE_SEARCH;
                force_web_update = 1;
            }
            Search_Mode();
        }

        // 3. TÍNH NĂNG CHỐNG ĐƠ 2 GIÂY (ANTI-STALL)
        if (TIM2->CCR2 == 0 && TIM2->CCR3 == 0) {
            // Nếu bị đứng im quá 2000ms (2 giây)
            if (HAL_GetTick() - last_active_time > 2000) {
                Motor_SetSpeed(&motorL, 500);  // Tiến
                Motor_SetSpeed(&motorR, -500); // Lùi -> Xoay phải nhẹ
                HAL_Delay(250);                // Xoay trong 0.25 giây để thoát
                last_active_time = HAL_GetTick(); // Reset lại đồng hồ
            }
        } else {
            // Nếu xe đang chạy bình thường, liên tục cập nhật đồng hồ
            last_active_time = HAL_GetTick();
        }

        // 4. BẮN GÓI TIN TELEMETRY LÊN WEB
        static uint32_t last_telemetry_time = 0;

        if ((HAL_GetTick() - last_telemetry_time >= 100) || force_web_update == 1) {
            printf("{\"type\": \"telemetry\", \"line\": [%d,%d,%d,%d,%d], \"e3f\": [%d,%d,%d,%d,%d,%d], \"motor_l\": %d, \"motor_r\": %d}\n",
                   LineArray.DigitalResults[0], LineArray.DigitalResults[1], LineArray.DigitalResults[2], LineArray.DigitalResults[3], LineArray.DigitalResults[4],
                   DistanceArray.Results[0], DistanceArray.Results[1], DistanceArray.Results[2], DistanceArray.Results[3], DistanceArray.Results[4], DistanceArray.Results[5],
                   (int)TIM2->CCR2, (int)TIM2->CCR3);

            Line_Write_Data(&LineArray);
            Distance_Write_Data(&DistanceArray);

            last_telemetry_time = HAL_GetTick();
            force_web_update = 0;
        }

        HAL_Delay(5);
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 12;
    RCC_OscInitStruct.PLL.PLLN = 96;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void Survival_Mode(void) {
    Motor_SetSpeed(&motorL, 0);
    Motor_SetSpeed(&motorR, 0);
    HAL_Delay(20);

    if (LineArray.DigitalResults[1] || LineArray.DigitalResults[2] || LineArray.DigitalResults[3]) {
        Motor_SetSpeed(&motorL, -800);
        Motor_SetSpeed(&motorR, -800);
        HAL_Delay(300);
        if (LineArray.DigitalResults[1]) {
            Motor_SetSpeed(&motorL, 600);
            Motor_SetSpeed(&motorR, -600);
        } else {
            Motor_SetSpeed(&motorL, -600);
            Motor_SetSpeed(&motorR, 600);
        }
        HAL_Delay(200);
    }
    else if (LineArray.DigitalResults[0] || LineArray.DigitalResults[4]) {
        Motor_SetSpeed(&motorL, 800);
        Motor_SetSpeed(&motorR, 800);
        HAL_Delay(300);
    }

    Motor_SetSpeed(&motorL, 0);
    Motor_SetSpeed(&motorR, 0);
}

void Attack_Mode(void) {
    if (DistanceArray.Results[2]) {
        Motor_SetSpeed(&motorL, 1000);
        Motor_SetSpeed(&motorR, 1000);
    }
    else if (DistanceArray.Results[1]) {
        Motor_SetSpeed(&motorL, 1000);
        Motor_SetSpeed(&motorR, 600);
    }
    else if (DistanceArray.Results[3]) {
        Motor_SetSpeed(&motorL, 600);
        Motor_SetSpeed(&motorR, 1000);
    }
}

void Defense_Mode(void) {
    if (DistanceArray.Results[0]) {
        Motor_SetSpeed(&motorL, -1000);
        Motor_SetSpeed(&motorR, 1000);
    }
    else if (DistanceArray.Results[4]) {
        Motor_SetSpeed(&motorL, 1000);
        Motor_SetSpeed(&motorR, -1000);
    }
    else if (DistanceArray.Results[5]) {
        Motor_SetSpeed(&motorL, 1000);
        Motor_SetSpeed(&motorR, -1000);
    }
}

void Search_Mode(void) {
    Motor_SetSpeed(&motorL, 1000);
    Motor_SetSpeed(&motorR, -1000);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
