################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LOG/Log.cpp \
../LOG/main.cpp 

O_SRCS += \
../LOG/Log.o \
../LOG/main.o 

OBJS += \
./LOG/Log.o \
./LOG/main.o 

CPP_DEPS += \
./LOG/Log.d \
./LOG/main.d 


# Each subdirectory must supply rules for building sources it contributes
LOG/%.o: ../LOG/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/empmessage" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -I/BaseEndPoint_EmpMessage/DYN_CONFIG/include -I"/home/araju/workspace/BaseEndPoint_EmpMessage" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


