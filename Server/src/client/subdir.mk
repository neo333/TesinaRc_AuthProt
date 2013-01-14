################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/client/main.cpp 

OBJS += \
./src/client/main.o 

CPP_DEPS += \
./src/client/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/client/%.o: ../src/client/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/neo/Repository/TesinaRc_AuthProt/src/comm" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


