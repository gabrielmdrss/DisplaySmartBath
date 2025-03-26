################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/liquidcrystal/liquidcrystal_i2c.c 

OBJS += \
./Drivers/liquidcrystal/liquidcrystal_i2c.o 

C_DEPS += \
./Drivers/liquidcrystal/liquidcrystal_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/liquidcrystal/%.o Drivers/liquidcrystal/%.su Drivers/liquidcrystal/%.cyclo: ../Drivers/liquidcrystal/%.c Drivers/liquidcrystal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/Spacefab-04/STM32CubeIDE/workspace_1.16.1/teste/Drivers/liquidcrystal" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-liquidcrystal

clean-Drivers-2f-liquidcrystal:
	-$(RM) ./Drivers/liquidcrystal/liquidcrystal_i2c.cyclo ./Drivers/liquidcrystal/liquidcrystal_i2c.d ./Drivers/liquidcrystal/liquidcrystal_i2c.o ./Drivers/liquidcrystal/liquidcrystal_i2c.su

.PHONY: clean-Drivers-2f-liquidcrystal

