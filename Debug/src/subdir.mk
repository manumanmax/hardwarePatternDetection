################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DisplayAnImage.cpp \
../src/Trainer.cpp 

OBJS += \
./src/DisplayAnImage.o \
./src/Trainer.o 

CPP_DEPS += \
./src/DisplayAnImage.d \
./src/Trainer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local -I/usr/local/include -I/usr/local/include/opencv -I/home/manu/workspace/Scanner/Includes -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


