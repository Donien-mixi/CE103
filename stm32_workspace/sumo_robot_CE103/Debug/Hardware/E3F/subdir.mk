################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Hardware/E3F/E3F.c 

OBJS += \
./Hardware/E3F/E3F.o 

C_DEPS += \
./Hardware/E3F/E3F.d 


# Each subdirectory must supply rules for building sources it contributes
Hardware/E3F/%.o Hardware/E3F/%.su Hardware/E3F/%.cyclo: ../Hardware/E3F/%.c Hardware/E3F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32_workspace/sumo_robot_CE103/Hardware" -I"D:/stm32_workspace/sumo_robot_CE103/Hardware/TCRT5000" -I"D:/stm32_workspace/sumo_robot_CE103/Hardware/Motor" -I"D:/stm32_workspace/sumo_robot_CE103/Hardware/E3F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Hardware-2f-E3F

clean-Hardware-2f-E3F:
	-$(RM) ./Hardware/E3F/E3F.cyclo ./Hardware/E3F/E3F.d ./Hardware/E3F/E3F.o ./Hardware/E3F/E3F.su

.PHONY: clean-Hardware-2f-E3F

