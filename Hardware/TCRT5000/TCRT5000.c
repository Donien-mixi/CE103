#include "TCRT5000.h"

/**
 * @brief Khởi tạo mảng cảm biến
 */
void LineArrayADC_Init(LineArrayADC_t *array, ADC_HandleTypeDef* hadc, uint16_t threshold) {
    array->AdcHandle = hadc;
    array->Threshold = threshold;

    // Bắt đầu cho phép ADC lấy mẫu liên tục và đẩy vào RAM qua DMA
    // Lưu ý: Số lượng chuyển đổi (Number of Conversion) trong CubeMX phải khớp với SENSOR_COUNT
    HAL_ADC_Start_DMA(array->AdcHandle, (uint32_t*)array->RawData, LINE_SENSOR_COUNT);
}

void LineArrayADC_UpdateLogic(LineArrayADC_t *array) {
    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
        // Ví dụ: Nếu giá trị ADC > Ngưỡng (Threshold) thì coi như phát hiện vạch/vật
        // Lưu ý: Tùy loại cảm biến mà logic có thể ngược lại ( < Threshold)
        if (array->RawData[i] > array->Threshold) {
            array->DigitalResults[i] = true;
        } else {
            array->DigitalResults[i] = false;
        }

        // Tính toán giá trị phần trăm (tùy chọn)
        array->CalibratedData[i] = (float)array->RawData[i] / 4095.0f;
    }
}

bool LineArrayADC_NoDetection(LineArrayADC_t *array) {
    for (int i = 0; i < LINE_SENSOR_COUNT; i++) {
        if (array->DigitalResults[i] == true) {
            return false;
        }
    }
    return true;
}
