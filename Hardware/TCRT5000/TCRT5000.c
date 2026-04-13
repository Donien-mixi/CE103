#include "TCRT5000.h"

/**
 * @brief Khởi tạo mảng cảm biến
 */
void LineArray_Init(LineSensorArray_t *array, GPIO_TypeDef** ports, uint16_t* pins, GPIO_PinState activeLevel) {
    array->ActiveLevel = activeLevel;
    for (int i = 0; i < SENSOR_COUNT; i++) {
        array->Sensors[i].Port = ports[i];
        array->Sensors[i].Pin = pins[i];
    }
}

/**
 * @brief Đọc 5 mắt cảm biến và trả về 1 byte duy nhất
 * Mỗi bit trong byte tương ứng với 1 mắt (Bit 0: Cảm biến 1, Bit 1: Cảm biến 2,...)
 */
uint8_t LineArray_Read(LineSensorArray_t *array) {
    uint8_t result = 0;

    for (int i = 0; i < SENSOR_COUNT; i++) {
        if (HAL_GPIO_ReadPin(array->Sensors[i].Port, array->Sensors[i].Pin) == array->ActiveLevel) {
            result |= (1 << i); // Ghi bit 1 vào vị trí tương ứng nếu thấy vạch
        }
    }

    array->SensorResult = result;
    return result;
}
