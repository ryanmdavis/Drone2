################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../commands.c \
../misc.c \
../prototypes.c \
../state.c \
../throttle.c 

OBJS += \
./commands.o \
./misc.o \
./prototypes.o \
./state.o \
./throttle.o 

C_DEPS += \
./commands.d \
./misc.d \
./prototypes.d \
./state.d \
./throttle.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


