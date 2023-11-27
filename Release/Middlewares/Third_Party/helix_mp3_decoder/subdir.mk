################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/helix_mp3_decoder/bitstream.c \
../Middlewares/Third_Party/helix_mp3_decoder/buffers.c \
../Middlewares/Third_Party/helix_mp3_decoder/dct32.c \
../Middlewares/Third_Party/helix_mp3_decoder/dequant.c \
../Middlewares/Third_Party/helix_mp3_decoder/dqchan.c \
../Middlewares/Third_Party/helix_mp3_decoder/huffman.c \
../Middlewares/Third_Party/helix_mp3_decoder/hufftabs.c \
../Middlewares/Third_Party/helix_mp3_decoder/imdct.c \
../Middlewares/Third_Party/helix_mp3_decoder/mp3dec.c \
../Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.c \
../Middlewares/Third_Party/helix_mp3_decoder/polyphase.c \
../Middlewares/Third_Party/helix_mp3_decoder/scalfact.c \
../Middlewares/Third_Party/helix_mp3_decoder/stproc.c \
../Middlewares/Third_Party/helix_mp3_decoder/subband.c \
../Middlewares/Third_Party/helix_mp3_decoder/trigtabs.c 

OBJS += \
./Middlewares/Third_Party/helix_mp3_decoder/bitstream.o \
./Middlewares/Third_Party/helix_mp3_decoder/buffers.o \
./Middlewares/Third_Party/helix_mp3_decoder/dct32.o \
./Middlewares/Third_Party/helix_mp3_decoder/dequant.o \
./Middlewares/Third_Party/helix_mp3_decoder/dqchan.o \
./Middlewares/Third_Party/helix_mp3_decoder/huffman.o \
./Middlewares/Third_Party/helix_mp3_decoder/hufftabs.o \
./Middlewares/Third_Party/helix_mp3_decoder/imdct.o \
./Middlewares/Third_Party/helix_mp3_decoder/mp3dec.o \
./Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.o \
./Middlewares/Third_Party/helix_mp3_decoder/polyphase.o \
./Middlewares/Third_Party/helix_mp3_decoder/scalfact.o \
./Middlewares/Third_Party/helix_mp3_decoder/stproc.o \
./Middlewares/Third_Party/helix_mp3_decoder/subband.o \
./Middlewares/Third_Party/helix_mp3_decoder/trigtabs.o 

C_DEPS += \
./Middlewares/Third_Party/helix_mp3_decoder/bitstream.d \
./Middlewares/Third_Party/helix_mp3_decoder/buffers.d \
./Middlewares/Third_Party/helix_mp3_decoder/dct32.d \
./Middlewares/Third_Party/helix_mp3_decoder/dequant.d \
./Middlewares/Third_Party/helix_mp3_decoder/dqchan.d \
./Middlewares/Third_Party/helix_mp3_decoder/huffman.d \
./Middlewares/Third_Party/helix_mp3_decoder/hufftabs.d \
./Middlewares/Third_Party/helix_mp3_decoder/imdct.d \
./Middlewares/Third_Party/helix_mp3_decoder/mp3dec.d \
./Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.d \
./Middlewares/Third_Party/helix_mp3_decoder/polyphase.d \
./Middlewares/Third_Party/helix_mp3_decoder/scalfact.d \
./Middlewares/Third_Party/helix_mp3_decoder/stproc.d \
./Middlewares/Third_Party/helix_mp3_decoder/subband.d \
./Middlewares/Third_Party/helix_mp3_decoder/trigtabs.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/helix_mp3_decoder/%.o Middlewares/Third_Party/helix_mp3_decoder/%.su: ../Middlewares/Third_Party/helix_mp3_decoder/%.c Middlewares/Third_Party/helix_mp3_decoder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FatFs/src/drivers -I"E:/Coding/STM32CubeIDE/STM32-StreamDeck/Middlewares/Third_Party/helix_mp3_decoder" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-helix_mp3_decoder

clean-Middlewares-2f-Third_Party-2f-helix_mp3_decoder:
	-$(RM) ./Middlewares/Third_Party/helix_mp3_decoder/bitstream.d ./Middlewares/Third_Party/helix_mp3_decoder/bitstream.o ./Middlewares/Third_Party/helix_mp3_decoder/bitstream.su ./Middlewares/Third_Party/helix_mp3_decoder/buffers.d ./Middlewares/Third_Party/helix_mp3_decoder/buffers.o ./Middlewares/Third_Party/helix_mp3_decoder/buffers.su ./Middlewares/Third_Party/helix_mp3_decoder/dct32.d ./Middlewares/Third_Party/helix_mp3_decoder/dct32.o ./Middlewares/Third_Party/helix_mp3_decoder/dct32.su ./Middlewares/Third_Party/helix_mp3_decoder/dequant.d ./Middlewares/Third_Party/helix_mp3_decoder/dequant.o ./Middlewares/Third_Party/helix_mp3_decoder/dequant.su ./Middlewares/Third_Party/helix_mp3_decoder/dqchan.d ./Middlewares/Third_Party/helix_mp3_decoder/dqchan.o ./Middlewares/Third_Party/helix_mp3_decoder/dqchan.su ./Middlewares/Third_Party/helix_mp3_decoder/huffman.d ./Middlewares/Third_Party/helix_mp3_decoder/huffman.o ./Middlewares/Third_Party/helix_mp3_decoder/huffman.su ./Middlewares/Third_Party/helix_mp3_decoder/hufftabs.d ./Middlewares/Third_Party/helix_mp3_decoder/hufftabs.o ./Middlewares/Third_Party/helix_mp3_decoder/hufftabs.su ./Middlewares/Third_Party/helix_mp3_decoder/imdct.d ./Middlewares/Third_Party/helix_mp3_decoder/imdct.o ./Middlewares/Third_Party/helix_mp3_decoder/imdct.su ./Middlewares/Third_Party/helix_mp3_decoder/mp3dec.d ./Middlewares/Third_Party/helix_mp3_decoder/mp3dec.o ./Middlewares/Third_Party/helix_mp3_decoder/mp3dec.su ./Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.d ./Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.o ./Middlewares/Third_Party/helix_mp3_decoder/mp3tabs.su ./Middlewares/Third_Party/helix_mp3_decoder/polyphase.d ./Middlewares/Third_Party/helix_mp3_decoder/polyphase.o ./Middlewares/Third_Party/helix_mp3_decoder/polyphase.su ./Middlewares/Third_Party/helix_mp3_decoder/scalfact.d ./Middlewares/Third_Party/helix_mp3_decoder/scalfact.o ./Middlewares/Third_Party/helix_mp3_decoder/scalfact.su ./Middlewares/Third_Party/helix_mp3_decoder/stproc.d ./Middlewares/Third_Party/helix_mp3_decoder/stproc.o ./Middlewares/Third_Party/helix_mp3_decoder/stproc.su ./Middlewares/Third_Party/helix_mp3_decoder/subband.d ./Middlewares/Third_Party/helix_mp3_decoder/subband.o ./Middlewares/Third_Party/helix_mp3_decoder/subband.su ./Middlewares/Third_Party/helix_mp3_decoder/trigtabs.d ./Middlewares/Third_Party/helix_mp3_decoder/trigtabs.o ./Middlewares/Third_Party/helix_mp3_decoder/trigtabs.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-helix_mp3_decoder

