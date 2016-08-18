################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../IPC_UDS/UnixDomainSocket.cpp 

OBJS += \
./IPC_UDS/UnixDomainSocket.o 

CPP_DEPS += \
./IPC_UDS/UnixDomainSocket.d 


# Each subdirectory must supply rules for building sources it contributes
IPC_UDS/%.o: ../IPC_UDS/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/araju/workspace/empmessage" -I/software/gtest/x86_64/1.7.0/include -I/usr/include/oracle/11.2/client64 -I/BaseEndPoint_EmpMessage/DYN_CONFIG/include -I"/home/araju/workspace/BaseEndPoint_EmpMessage" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


