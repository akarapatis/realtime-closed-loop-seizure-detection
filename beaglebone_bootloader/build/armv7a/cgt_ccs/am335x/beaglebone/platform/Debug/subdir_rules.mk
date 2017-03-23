################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
board.obj: /home/thanasis/projects/starterwarefree-code/platform/evmAM335x/board.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="board.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

cpsw.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/cpsw.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="cpsw.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

ctlregcontext.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/ctlregcontext.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="ctlregcontext.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

dmtimer.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/dmtimer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="dmtimer.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

edma.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/edma.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="edma.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

eeprom.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/eeprom.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="eeprom.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

gpio.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/gpio.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="gpio.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

hs_mmcsd.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/hs_mmcsd.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="hs_mmcsd.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

hsi2c.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/hsi2c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="hsi2c.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

rtc.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/rtc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="rtc.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

sysdelay.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/sysdelay.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="sysdelay.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

sysperf.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/sysperf.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="sysperf.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

timertick.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/timertick.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="timertick.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

uartConsole.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/uartConsole.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="uartConsole.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

usb.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/usb.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="usb.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

watchdog.obj: /home/thanasis/projects/starterwarefree-code/platform/beaglebone/watchdog.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../../../include/armv7a/am335x" --include_path="../../../../../../../include/hw" --include_path="../../../../../../../include" --gcc --define=DMA_MODE --diag_warning=225 --display_error_number --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="watchdog.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


