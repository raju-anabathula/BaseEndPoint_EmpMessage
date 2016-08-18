################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include TEST/subdir.mk
-include LOG/subdir.mk
-include IPC_UDS/subdir.mk
-include IPC_PIPE/subdir.mk
-include IPC_FIFO/subdir.mk
-include COMMON/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BaseEndPoint_EmpMessage

dependents:
	-cd /home/araju/workspace/empmessage/Release && $(MAKE) all

# Tool invocations
BaseEndPoint_EmpMessage: $(OBJS) $(USER_OBJS) /home/araju/workspace/empmessage/Release/libempmessage.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/araju/workspace/empmessage/Release" -L/software/gtest/x86_64/1.7.0/lib -L/usr/lib/oracle/11.2/client64/lib -L"/home/araju/workspace/BaseEndPoint_EmpMessage/DYN_CONFIG/lib" -o "BaseEndPoint_EmpMessage" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) BaseEndPoint_EmpMessage
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/araju/workspace/empmessage/Release/libempmessage.a:

-include ../makefile.targets
