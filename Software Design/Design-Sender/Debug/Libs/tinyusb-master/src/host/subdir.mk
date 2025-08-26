################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/tinyusb-master/src/host/hub.c \
../Libs/tinyusb-master/src/host/usbh.c 

OBJS += \
./Libs/tinyusb-master/src/host/hub.o \
./Libs/tinyusb-master/src/host/usbh.o 

C_DEPS += \
./Libs/tinyusb-master/src/host/hub.d \
./Libs/tinyusb-master/src/host/usbh.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/tinyusb-master/src/host/%.o Libs/tinyusb-master/src/host/%.su Libs/tinyusb-master/src/host/%.cyclo: ../Libs/tinyusb-master/src/host/%.c Libs/tinyusb-master/src/host/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/src" -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/hw" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-host

clean-Libs-2f-tinyusb-2d-master-2f-src-2f-host:
	-$(RM) ./Libs/tinyusb-master/src/host/hub.cyclo ./Libs/tinyusb-master/src/host/hub.d ./Libs/tinyusb-master/src/host/hub.o ./Libs/tinyusb-master/src/host/hub.su ./Libs/tinyusb-master/src/host/usbh.cyclo ./Libs/tinyusb-master/src/host/usbh.d ./Libs/tinyusb-master/src/host/usbh.o ./Libs/tinyusb-master/src/host/usbh.su

.PHONY: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-host

