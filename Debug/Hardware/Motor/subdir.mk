################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/Motor/Motor.c 

OBJS += \
./Hardware/Motor/Motor.o 

C_DEPS += \
./Hardware/Motor/Motor.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/Motor/%.o Hardware/Motor/%.su Hardware/Motor/%.cyclo: ../Hardware/Motor/%.c Hardware/Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/HK4/VXL_VDK/ROBOT_SUMO/Sensor/test_sensor/Hardware" -I"D:/HK4/VXL_VDK/ROBOT_SUMO/Sensor/test_sensor/Hardware/TCRT5000" -I"D:/HK4/VXL_VDK/ROBOT_SUMO/Sensor/test_sensor/Hardware/Motor" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-Motor

clean-Hardware-2f-Motor:
	-$(RM) ./Hardware/Motor/Motor.cyclo ./Hardware/Motor/Motor.d ./Hardware/Motor/Motor.o ./Hardware/Motor/Motor.su

.PHONY: clean-Hardware-2f-Motor

