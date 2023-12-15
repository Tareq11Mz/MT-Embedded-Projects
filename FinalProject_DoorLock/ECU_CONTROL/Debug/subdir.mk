################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../Timer_1.c \
../UART.c \
../dcMotor.c \
../eeprom.c \
../gpio.c \
../i2c.c \
../main.c \
../pwm.c 

OBJS += \
./Buzzer.o \
./Timer_1.o \
./UART.o \
./dcMotor.o \
./eeprom.o \
./gpio.o \
./i2c.o \
./main.o \
./pwm.o 

C_DEPS += \
./Buzzer.d \
./Timer_1.d \
./UART.d \
./dcMotor.d \
./eeprom.d \
./gpio.d \
./i2c.d \
./main.d \
./pwm.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


