################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/tinyusb-master/src/common/tusb_fifo.c 

OBJS += \
./Libs/tinyusb-master/src/common/tusb_fifo.o 

C_DEPS += \
./Libs/tinyusb-master/src/common/tusb_fifo.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/tinyusb-master/src/common/%.o Libs/tinyusb-master/src/common/%.su Libs/tinyusb-master/src/common/%.cyclo: ../Libs/tinyusb-master/src/common/%.c Libs/tinyusb-master/src/common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/src" -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/hw" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-common

clean-Libs-2f-tinyusb-2d-master-2f-src-2f-common:
	-$(RM) ./Libs/tinyusb-master/src/common/tusb_fifo.cyclo ./Libs/tinyusb-master/src/common/tusb_fifo.d ./Libs/tinyusb-master/src/common/tusb_fifo.o ./Libs/tinyusb-master/src/common/tusb_fifo.su

.PHONY: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-common

