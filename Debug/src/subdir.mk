################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Composant.cpp \
../src/Corners.cpp \
../src/DisplayAnImage.cpp \
../src/Launcher.cpp \
../src/Pattern.cpp \
../src/Scene.cpp 

OBJS += \
./src/Composant.o \
./src/Corners.o \
./src/DisplayAnImage.o \
./src/Launcher.o \
./src/Pattern.o \
./src/Scene.o 

CPP_DEPS += \
./src/Composant.d \
./src/Corners.d \
./src/DisplayAnImage.d \
./src/Launcher.d \
./src/Pattern.d \
./src/Scene.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/local/include -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


