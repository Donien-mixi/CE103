/*
 * E3F.h
 *
 *  Created on: Apr 14, 2026
 *      Author: Dell
 */

#ifndef E3F_E3F_H_
#define E3F_E3F_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


#define E3F_COUNT 6

typedef struct {
    GPIO_TypeDef* Port;
    uint16_t Pin;
} E3F_PinConfig_t;

typedef struct {
    E3F_PinConfig_t Sensors[E3F_COUNT];
    bool Results[E3F_COUNT];    // Lưu kết quả dưới dạng mảng bool cho dễ code
    GPIO_PinState ActiveLevel;  // Mức logic khi cảm biến phát hiện vật (thường là RESET cho NPN)
} E3FArray_t;

// Khởi tạo dãy cảm biến
void E3FArray_Init(E3FArray_t *array, GPIO_TypeDef** ports, uint16_t* pins, GPIO_PinState activeLevel);

// Cập nhật trạng thái toàn bộ dãy cảm biến
void E3FArray_Update(E3FArray_t *array);


// Kiểm tra xem CẢ DÀN có đang TRỐNG (không phát hiện gì) hay không
bool E3FArray_NoDetection(E3FArray_t *array);

void Distance_Write_Data(E3FArray_t *array);


#endif /* E3F_E3F_H_ */
