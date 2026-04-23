//#ifndef TCRT5000_H
//#define TCRT5000_H
//
//#include "stm32f4xx_hal.h" // Thay đổi theo dòng chip (f1, f4, g4...)
//#include <stdbool.h>
//
//#define LINE_SENSOR_COUNT 5
//
//typedef struct {
//    ADC_HandleTypeDef* AdcHandle;
//    uint32_t RawData[LINE_SENSOR_COUNT]; // Mảng lưu giá trị ADC thô từ DMA
//    float CalibratedData[LINE_SENSOR_COUNT]; // Giá trị sau khi chuẩn hóa (0.0 - 1.0 hoặc 0 - 100)
//    uint16_t Threshold;             // Ngưỡng để quyết định có phát hiện vật hay không
//    bool DigitalResults[LINE_SENSOR_COUNT]; // Kết quả logic sau khi so sánh ngưỡng
//} LineArrayADC_t;
//
//// Khởi tạo và bắt đầu quá trình DMA
//void LineArrayADC_Init(LineArrayADC_t *array, ADC_HandleTypeDef* hadc, uint16_t threshold);
//
//// Cập nhật kết quả logic từ dữ liệu ADC
//void LineArrayADC_UpdateLogic(LineArrayADC_t *array);
//
//// Kiểm tra xem cả dàn có đang trống (không phát hiện gì) không
//bool LineArrayADC_NoDetection(LineArrayADC_t *array);
//
//void Write_Raw_Data(LineArrayADC_t *array);
//
//#endif


#ifndef TCRT5000_H
#define TCRT5000_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define LINE_SENSOR_COUNT 5

typedef struct {
    GPIO_TypeDef* GPIO_Port[LINE_SENSOR_COUNT]; // Mảng lưu Port (GPIOA, GPIOB...)
    uint16_t      GPIO_Pin[LINE_SENSOR_COUNT];  // Mảng lưu Pin (GPIO_PIN_0...)
    bool          DigitalResults[LINE_SENSOR_COUNT]; // Kết quả Logic (0 hoặc 1)
    bool          ActiveLow; // true nếu cảm biến trả về 0 khi gặp vạch đen
} LineArray_t;

// Khởi tạo danh sách các chân GPIO
void Line_Init(LineArray_t *array, bool activeLow);

// Đọc trạng thái từ các chân Digital
void Line_Update(LineArray_t *array);

// Kiểm tra xem có đang không thấy vạch nào không
bool Line_NoDetection(LineArray_t *array);

// In dữ liệu ra màn hình Debug
void Line_Write_Data(LineArray_t *array);

#endif
