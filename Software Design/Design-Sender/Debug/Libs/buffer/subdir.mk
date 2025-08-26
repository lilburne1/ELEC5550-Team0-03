################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/buffer/tm_stm32_buffer.c 

OBJS += \
./Libs/buffer/tm_stm32_buffer.o 

C_DEPS += \
./Libs/buffer/tm_stm32_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/buffer/%.o Libs/buffer/%.su Libs/buffer/%.cyclo: ../Libs/buffer/%.c Libs/buffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/src" -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/hw" -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/buffer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-buffer

clean-Libs-2f-buffer:
	-$(RM) ./Libs/buffer/tm_stm32_buffer.cyclo ./Libs/buffer/tm_stm32_buffer.d ./Libs/buffer/tm_stm32_buffer.o ./Libs/buffer/tm_stm32_buffer.su

.PHONY: clean-Libs-2f-buffer

