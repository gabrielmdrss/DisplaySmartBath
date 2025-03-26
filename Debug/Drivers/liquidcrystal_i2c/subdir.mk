################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/liquidcrystal_i2c/liquidcrystal_i2c.c 

OBJS += \
./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.o 

C_DEPS += \
./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/liquidcrystal_i2c/%.o Drivers/liquidcrystal_i2c/%.su Drivers/liquidcrystal_i2c/%.cyclo: ../Drivers/liquidcrystal_i2c/%.c Drivers/liquidcrystal_i2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/Spacefab-04/STM32CubeIDE/workspace_1.16.1/teste/Drivers/liquidcrystal_i2c" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-liquidcrystal_i2c

clean-Drivers-2f-liquidcrystal_i2c:
	-$(RM) ./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.cyclo ./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.d ./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.o ./Drivers/liquidcrystal_i2c/liquidcrystal_i2c.su

.PHONY: clean-Drivers-2f-liquidcrystal_i2c

