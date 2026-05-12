/*
 * E3F.c
 *
 *  Created on: Apr 14, 2026
 *      Author: Dell
 */

#include "E3F.h"
#include <stdio.h>



void E3FArray_Init(E3FArray_t *array, GPIO_TypeDef** ports, uint16_t* pins, GPIO_PinState activeLevel) {
    array->ActiveLevel = activeLevel;

    for (int i = 0; i < E3F_COUNT; i++) {
        array->Sensors[i].Port = ports[i];
        array->Sensors[i].Pin = pins[i];
        array->Results[i] = !activeLevel;
    }
}

void E3FArray_Update(E3FArray_t *array) {
    for (int i = 0; i < E3F_COUNT; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(array->Sensors[i].Port, array->Sensors[i].Pin);

        // Trả về true nếu trạng thái chân khớp với mức kích hoạt (ActiveLevel)
        array->Results[i] = (state == array->ActiveLevel);
    }
}


bool E3FArray_NoDetection(E3FArray_t *array) {
    for (int i = 0; i < E3F_COUNT; i++) {
        // Nếu có bất kỳ con nào phát hiện (true), thì không thể gọi là "No Detection"
        if (array->Results[i] == true) {
            return false;
        }
    }
    return true; // Tất cả đều false
}

void Distance_Write_Data(E3FArray_t *array) {
    printf("Distance Logic: ");
    for (int i = 0; i < E3F_COUNT; i++) {
        printf("%d ", array->Results[i] ? 1 : 0);
    }
    printf("\r\n");
}
