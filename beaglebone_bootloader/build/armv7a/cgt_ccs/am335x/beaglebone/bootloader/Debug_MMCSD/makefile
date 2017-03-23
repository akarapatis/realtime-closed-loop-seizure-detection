################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./bl_copy.obj" \
"./bl_hsmmcsd.obj" \
"./bl_init.obj" \
"./bl_main.obj" \
"./bl_platform.obj" \
"./cache.obj" \
"./cp15.obj" \
"./device.obj" \
"./fat_mmcsd.obj" \
"./ff.obj" \
"./mmu.obj" \
"../boot.cmd" \
-l"libc.a" \
-l"../../../../../../../binary/armv7a/cgt_ccs/utils/Debug/utils.lib" \
-l"../../../../../../../binary/armv7a/cgt_ccs/mmcsdlib/Debug/libmmcsd.lib" \
-l"../../../../../../../binary/armv7a/cgt_ccs/am335x/drivers/Debug/drivers.lib" \
-l"../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/platform/Debug/platform.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out \

EXE_OUTPUTS__QUOTED += \
"../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out" \


# All Target
all: ../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out

# Tool invocations
../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me -g --gcc --define=am335x --define=beaglebone --define=MMCSD --diag_warning=225 --display_error_number --diag_wrap=off -z -m"boot.map" -i"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/lib" -i"/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="boot_linkInfo.xml" --ram_model -o "../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "bl_copy.pp" "bl_hsmmcsd.pp" "bl_main.pp" "bl_platform.pp" "cache.pp" "device.pp" "fat_mmcsd.pp" "ff.pp" "mmu.pp" 
	-$(RM) "bl_copy.obj" "bl_hsmmcsd.obj" "bl_init.obj" "bl_main.obj" "bl_platform.obj" "cache.obj" "cp15.obj" "device.obj" "fat_mmcsd.obj" "ff.obj" "mmu.obj" 
	-$(RM) "bl_init.pp" "cp15.pp" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"/home/thanasis/Programs/ti/ccsv6/utils/tiobj2bin/tiobj2bin"  "../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.out"  "../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.bin"  "/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armofd"  "/home/thanasis/Programs/ti/ccsv6/tools/compiler/arm_5.1.10/bin/armhex"  "/home/thanasis/Programs/ti/ccsv6/utils/tiobj2bin/mkhex4bin"
	-"../../../../../../../tools/ti_image/tiimage" "0x402F0400" "MMCSD" "../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/boot.bin" "../../../../../../../binary/armv7a/cgt_ccs/am335x/beaglebone/bootloader/Debug_MMCSD/MLO"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

