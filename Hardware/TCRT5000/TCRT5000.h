#ifndef TCRT5000_H
#define TCRT5000_H

#include "stm32f4xx_hal.h"

#define SENSOR_COUNT 5

// Cấu trúc cho từng mắt cảm biến lẻ
typedef struct {
    GPIO_TypeDef* Port;
    uint16_t Pin;
} SensorPin_t;

//				  Trước
//					S2
//				S1		S3
//	(Trái)	S0				S4	(Phải)
//


typedef struct {
    SensorPin_t Sensors[SENSOR_COUNT];
    GPIO_PinState ActiveLevel;  // Mức logic khi thấy vạch (SET/RESET)
    uint8_t SensorResult;       // Lưu kết quả dưới dạng bitmask (VD: 0b00011100)
} LineSensorArray_t;

// Khởi tạo dàn cảm biến
void LineArray_Init(LineSensorArray_t *array, GPIO_TypeDef** ports, uint16_t* pins, GPIO_PinState activeLevel);

// Đọc toàn bộ 5 mắt cảm biến cùng lúc
uint8_t LineArray_Read(LineSensorArray_t *array);

#endif
