################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/srccpp/Dwin/Dwinhmi.cpp 

OBJS += \
./Core/srccpp/Dwin/Dwinhmi.o 

CPP_DEPS += \
./Core/srccpp/Dwin/Dwinhmi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/srccpp/Dwin/%.o Core/srccpp/Dwin/%.su: ../Core/srccpp/Dwin/%.cpp Core/srccpp/Dwin/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L433xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/SourceTree/janatics/Core/srccpp/Bussinesslogic" -I"D:/SourceTree/janatics/Core/srccpp/Common" -I"D:/SourceTree/janatics/Core/srccpp/DisplayCPP" -I"D:/SourceTree/janatics/Core/srccpp/Ethernet" -I"D:/SourceTree/janatics/Core/srccpp/Modbus" -I"D:/SourceTree/janatics/Core/srccpp/OfflineStorage" -I"D:/SourceTree/janatics/Core/srccpp/Shift" -I"D:/SourceTree/janatics/Core/srccpp/WifiEsp8266" -I"D:/SourceTree/janatics/Core/srccpp" -I"D:/SourceTree/janatics/Core/srccpp/Dwin" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-srccpp-2f-Dwin

clean-Core-2f-srccpp-2f-Dwin:
	-$(RM) ./Core/srccpp/Dwin/Dwinhmi.d ./Core/srccpp/Dwin/Dwinhmi.o ./Core/srccpp/Dwin/Dwinhmi.su

.PHONY: clean-Core-2f-srccpp-2f-Dwin

