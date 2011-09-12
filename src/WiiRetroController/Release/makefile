################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Wire/utility/subdir.mk
-include Wire/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
WiiRetroController.lss \

FLASH_IMAGE += \
WiiRetroController.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: WiiRetroController.elf secondary-outputs

# Tool invocations
WiiRetroController.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc -Wl,--gc-sections -s -Os -o"WiiRetroController.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,WiiRetroController.map,--cref -L"D:\Arquivos de Programas\eclipse\workspace\DuemilanoveCore8Mhz\Release" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

WiiRetroController.lss: WiiRetroController.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S WiiRetroController.elf  >"WiiRetroController.lss"
	@echo 'Finished building: $@'
	@echo ' '

WiiRetroController.hex: WiiRetroController.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex WiiRetroController.elf  "WiiRetroController.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: WiiRetroController.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p WiiRetroController.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) WiiRetroController.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
