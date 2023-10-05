################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/srccpp/Bussinesslogic/LpdcLogic.cpp 

OBJS += \
./Core/srccpp/Bussinesslogic/LpdcLogic.o 

CPP_DEPS += \
./Core/srccpp/Bussinesslogic/LpdcLogic.d 


# Each subdirectory must supply rules for building sources it contributes
Core/srccpp/Bussinesslogic/%.o Core/srccpp/Bussinesslogic/%.su: ../Core/srccpp/Bussinesslogic/%.cpp Core/srccpp/Bussinesslogic/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L433xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/Bussinesslogic" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/Common" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/DisplayCPP" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/Ethernet" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/Modbus" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/OfflineStorage" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/Shift" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp/WifiEsp8266" -I"D:/PROGRAM/Naveen/Janatics/JanaticsIot_27_7_23_postMethod - URL Change - Copy/Core/srccpp" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-srccpp-2f-Bussinesslogic

clean-Core-2f-srccpp-2f-Bussinesslogic:
	-$(RM) ./Core/srccpp/Bussinesslogic/LpdcLogic.d ./Core/srccpp/Bussinesslogic/LpdcLogic.o ./Core/srccpp/Bussinesslogic/LpdcLogic.su

.PHONY: clean-Core-2f-srccpp-2f-Bussinesslogic

