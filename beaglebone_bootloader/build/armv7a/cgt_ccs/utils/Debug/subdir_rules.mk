################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ascii.obj: /home/thanasis/projects/starterwarefree-code/utils/ascii.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="ascii.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

board.obj: /home/thanasis/projects/starterwarefree-code/utils/board.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="board.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

cmdline.obj: /home/thanasis/projects/starterwarefree-code/utils/cmdline.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="cmdline.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

consoleUtils.obj: /home/thanasis/projects/starterwarefree-code/utils/consoleUtils.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="consoleUtils.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

delay.obj: /home/thanasis/projects/starterwarefree-code/utils/delay.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="delay.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

misc.obj: /home/thanasis/projects/starterwarefree-code/utils/misc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="misc.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

perf.obj: /home/thanasis/projects/starterwarefree-code/utils/perf.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="perf.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

systick.obj: /home/thanasis/projects/starterwarefree-code/utils/systick.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="systick.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

uartStdio.obj: /home/thanasis/projects/starterwarefree-code/utils/uartStdio.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --include_path="/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --include_path="../../../../../include" --gcc --diag_warning=225 --unaligned_access=on --enum_type=packed --preproc_with_compile --preproc_dependency="uartStdio.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


