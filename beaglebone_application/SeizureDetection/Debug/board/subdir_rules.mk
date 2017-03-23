################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_7-2013q3/bin/arm-none-eabi-gcc" -c -mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a -marm -mfloat-abi=softfp -mfpu=neon -Dam3359 -Dgcc -I"/home/thanasis/Programs/ti/ccsv6/tools/compiler/gcc-arm-none-eabi-4_7-2013q3/arm-none-eabi/include" -I"/home/thanasis/Programs/ti/ccsv5/tools/compiler/gcc-arm-none-eabi-4_7-2012q4/lib/gcc/arm-none-eabi/4.7.3/include" -I"../../../hw/beaglebone/include" -I"../../../hw/beaglebone/include/armv7a" -I"../../../hw/beaglebone/include/armv7a/am335x" -I"../../../hw/beaglebone/include/hw" -I"../../../hw/beaglebone/mmcsdlib/include" -I"../../../hw/beaglebone/third_party/fatfs/src" -O3 -g -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -ftree-vectorize -save-temps -Wextra -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


