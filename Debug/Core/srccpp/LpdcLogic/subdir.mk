################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/srccpp/LpdcLogic/LpdcLogic.cpp 

OBJS += \
./Core/srccpp/LpdcLogic/LpdcLogic.o 

CPP_DEPS += \
./Core/srccpp/LpdcLogic/LpdcLogic.d 


# Each subdirectory must supply rules for building sources it contributes
Core/srccpp/LpdcLogic/%.o Core/srccpp/LpdcLogic/%.su: ../Core/srccpp/LpdcLogic/%.cpp Core/srccpp/LpdcLogic/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L433xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/DisplayCPP" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/LpdcLogic" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/Common" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/Modbus" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/Ethernet" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/OfflineStorage" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/WifiEsp8266" -I"D:/Naveen_wrkspace/LPDC/24_2_2023_Lpdc_B1/Core/srccpp/Shift" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-srccpp-2f-LpdcLogic

clean-Core-2f-srccpp-2f-LpdcLogic:
	-$(RM) ./Core/srccpp/LpdcLogic/LpdcLogic.d ./Core/srccpp/LpdcLogic/LpdcLogic.o ./Core/srccpp/LpdcLogic/LpdcLogic.su

.PHONY: clean-Core-2f-srccpp-2f-LpdcLogic

