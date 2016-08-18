################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../IPC_PIPE/Pipe.cpp 

OBJS += \
./IPC_PIPE/Pipe.o 

CPP_DEPS += \
./IPC_PIPE/Pipe.d 


# Each subdirectory must supply rules for building sources it contributes
IPC_PIPE/%.o: ../IPC_PIPE/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/empmessage" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -I/BaseEndPoint_EmpMessage/DYN_CONFIG/include -I"/home/araju/workspace/BaseEndPoint_EmpMessage" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


