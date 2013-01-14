################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/server/AppProtServer.cpp \
../src/server/Server.cpp \
../src/server/main.cpp 

OBJS += \
./src/server/AppProtServer.o \
./src/server/Server.o \
./src/server/main.o 

CPP_DEPS += \
./src/server/AppProtServer.d \
./src/server/Server.d \
./src/server/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/server/%.o: ../src/server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/neo/Repository/TesinaRc_AuthProt/src/comm" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


