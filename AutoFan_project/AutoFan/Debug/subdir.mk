################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../dcMotor.c \
../gpio.c \
../lcd.c \
../lm35sensor.c \
../main.c \
../pwm.c 

OBJS += \
./adc.o \
./dcMotor.o \
./gpio.o \
./lcd.o \
./lm35sensor.o \
./main.o \
./pwm.o 

C_DEPS += \
./adc.d \
./dcMotor.d \
./gpio.d \
./lcd.d \
./lm35sensor.d \
./main.d \
./pwm.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


