################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Arquivos\ de\ Programas/arduino-0022/libraries/Wire/utility/twi.c 

OBJS += \
./Wire/utility/twi.o 

C_DEPS += \
./Wire/utility/twi.d 


# Each subdirectory must supply rules for building sources it contributes
Wire/utility/twi.o: D:/Arquivos\ de\ Programas/arduino-0022/libraries/Wire/utility/twi.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"D:/Arquivos de Programas/arduino-0022/hardware/arduino/cores/arduino" -I"D:/Arquivos de Programas/arduino-0022/libraries/Wire/utility" -I"D:/Arquivos de Programas/arduino-0022/libraries/Wire" -DARDUINO=18 -w -Os -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


