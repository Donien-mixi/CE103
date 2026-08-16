#include "Motor.h"

void Motor_Init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
                GPIO_TypeDef* port1, uint16_t pin1, GPIO_TypeDef* port2, uint16_t pin2) {
    motor->htim = htim;
    motor->channel = channel;
    motor->dirPort1 = port1;
    motor->dirPin1 = pin1;
    motor->dirPort2 = port2;
    motor->dirPin2 = pin2;

    HAL_TIM_PWM_Start(motor->htim, motor->channel);
    Motor_SetSpeed(motor, 0);
}

void Motor_SetSpeed(Motor_t* motor, int16_t speed) {
    // 1. Giới hạn tốc độ trong khoảng -1000 đến 1000
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;

    // 2. Điều khiển hướng (Chiều quay)
    if (speed > 0) {
        HAL_GPIO_WritePin(motor->dirPort1, motor->dirPin1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->dirPort2, motor->dirPin2, GPIO_PIN_RESET);
    } else if (speed < 0){
        HAL_GPIO_WritePin(motor->dirPort1, motor->dirPin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->dirPort2, motor->dirPin2, GPIO_PIN_SET);
        speed = -speed; // Chuyển speed về số dương để ghi vào PWM
    }
    else {
        HAL_GPIO_WritePin(motor->dirPort1, motor->dirPin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->dirPort2, motor->dirPin2, GPIO_PIN_RESET);
    }

    // 3. Ghi giá trị PWM (Duty Cycle)
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, speed);
}
