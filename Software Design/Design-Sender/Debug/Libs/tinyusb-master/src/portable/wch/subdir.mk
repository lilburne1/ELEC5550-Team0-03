################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.c \
../Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.c \
../Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.c 

OBJS += \
./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.o \
./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.o \
./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.o 

C_DEPS += \
./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.d \
./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.d \
./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/tinyusb-master/src/portable/wch/%.o Libs/tinyusb-master/src/portable/wch/%.su Libs/tinyusb-master/src/portable/wch/%.cyclo: ../Libs/tinyusb-master/src/portable/wch/%.c Libs/tinyusb-master/src/portable/wch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/src" -I"/Users/benlilburne/STM32CubeIDE/workspace_1.19.0/TinyUSB-Test/Libs/tinyusb-master/hw" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-portable-2f-wch

clean-Libs-2f-tinyusb-2d-master-2f-src-2f-portable-2f-wch:
	-$(RM) ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.cyclo ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.d ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.o ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbfs.su ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.cyclo ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.d ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.o ./Libs/tinyusb-master/src/portable/wch/dcd_ch32_usbhs.su ./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.cyclo ./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.d ./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.o ./Libs/tinyusb-master/src/portable/wch/hcd_ch32_usbfs.su

.PHONY: clean-Libs-2f-tinyusb-2d-master-2f-src-2f-portable-2f-wch

