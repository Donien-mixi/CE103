//#include "TCRT5000.h"
//
///**
// * @brief Khởi tạo mảng cảm biến
// */
//void LineArrayADC_Init(LineArrayADC_t *array, ADC_HandleTypeDef* hadc, uint16_t threshold) {
//    array->AdcHandle = hadc;
//    array->Threshold = threshold;
//
//    // Bắt đầu cho phép ADC lấy mẫu liên tục và đẩy vào RAM qua DMA
//    // Lưu ý: Số lượng chuyển đổi (Number of Conversion) trong CubeMX phải khớp với SENSOR_COUNT
//    HAL_ADC_Start_DMA(array->AdcHandle, (uint32_t*)array->RawData, LINE_SENSOR_COUNT);
//}
//
//void LineArrayADC_UpdateLogic(LineArrayADC_t *array) {
//    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
//        // Ví dụ: Nếu giá trị ADC > Ngưỡng (Threshold) thì coi như phát hiện vạch/vật
//        // Lưu ý: Tùy loại cảm biến mà logic có thể ngược lại ( < Threshold)
//        if (array->RawData[i] > array->Threshold) {
//            array->DigitalResults[i] = true;
//        } else {
//            array->DigitalResults[i] = false;
//        }
//
//        // Tính toán giá trị phần trăm (tùy chọn)
//        array->CalibratedData[i] = (float)array->RawData[i] / 4095.0f;
//    }
//}
//
//bool LineArrayADC_NoDetection(LineArrayADC_t *array) {
//    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
//        if (array->DigitalResults[i] == true) {
//            return false;
//        }
//    }
//    return true;
//}
//
//void Write_Raw_Data(LineArrayADC_t *array) {
//    printf("Raw: ");
//    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
//        // In giá trị ADC của từng cảm biến, cách nhau bởi dấu cách hoặc tab
//        printf("%4lu ", array->RawData[i]);
//    }
//
//    // In thêm trạng thái Logic để dễ quan sát (Tùy chọn)
//    printf(" | Logic: ");
//    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
//        printf("%d", array->DigitalResults[i] ? 1 : 0);
//    }
//
//    printf("\r\n"); // Xuống dòng sau khi in xong một lượt
//}
//


#include "TCRT5000.h"
#include <stdio.h>

/**
 * @brief Gán cứng các chân GPIO vào mảng
 */
void Line_Init(LineArray_t *array, bool activeLow) {
    array->ActiveLow = activeLow;

    // BẠN CẦN THAY ĐỔI CÁC CHÂN NÀY ĐÚNG VỚI THỰC TẾ TRÊN BOARD CỦA BẠN
    array->GPIO_Port[0] = GPIOA; array->GPIO_Pin[0] = GPIO_PIN_3; // Cảm biến 1
    array->GPIO_Port[1] = GPIOA; array->GPIO_Pin[1] = GPIO_PIN_4; // Cảm biến 2
    array->GPIO_Port[2] = GPIOA; array->GPIO_Pin[2] = GPIO_PIN_5; // Cảm biến 3
    array->GPIO_Port[3] = GPIOA; array->GPIO_Pin[3] = GPIO_PIN_6; // Cảm biến 4
    array->GPIO_Port[4] = GPIOB; array->GPIO_Pin[4] = GPIO_PIN_0; // Cảm biến 5
}

/**
 * @brief Đọc trạng thái Digital từ các chân GPIO
 */
void Line_Update(LineArray_t *array) {
    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(array->GPIO_Port[i], array->GPIO_Pin[i]);

        // Nếu ActiveLow = true: Sensor xuất 0 (RESET) khi gặp vạch -> DigitalResults = true
        if (array->ActiveLow) {
            array->DigitalResults[i] = (state == GPIO_PIN_RESET);
        } else {
            array->DigitalResults[i] = (state == GPIO_PIN_SET);
        }
    }
}

bool Line_NoDetection(LineArray_t *array) {
    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
        if (array->DigitalResults[i] == true) {
            return false;
        }
    }
    return true;
}

void Line_Write_Data(LineArray_t *array) {
    printf("Line Logic: ");
    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
        printf("%d ", array->DigitalResults[i] ? 1 : 0);
    }
    printf("\r\n");
}
