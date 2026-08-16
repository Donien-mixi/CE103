#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stm32f4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    GPIO_TypeDef* dirPort1;
    uint16_t dirPin1;
    GPIO_TypeDef* dirPort2;
    uint16_t dirPin2;
} Motor_t;

void Motor_Init(Motor_t* motor, TIM_HandleTypeDef* htim, uint32_t channel,
                GPIO_TypeDef* port1, uint16_t pin1, GPIO_TypeDef* port2, uint16_t pin2);

void Motor_SetSpeed(Motor_t* motor, int16_t speed);

#endif
