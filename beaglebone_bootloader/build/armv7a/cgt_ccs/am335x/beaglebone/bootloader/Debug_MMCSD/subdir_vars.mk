################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../boot.cmd 

ASM_SRCS += \
/home/thanasis/projects/starterwarefree-code/bootloader/src/armv7a/cgt/bl_init.asm \
/home/thanasis/projects/starterwarefree-code/system_config/armv7a/cgt/cp15.asm 

C_SRCS += \
/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_copy.c \
/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_hsmmcsd.c \
/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_main.c \
/home/thanasis/projects/starterwarefree-code/bootloader/src/armv7a/am335x/bl_platform.c \
/home/thanasis/projects/starterwarefree-code/system_config/armv7a/cache.c \
/home/thanasis/projects/starterwarefree-code/system_config/armv7a/am335x/device.c \
/home/thanasis/projects/starterwarefree-code/third_party/fatfs/port/fat_mmcsd.c \
/home/thanasis/projects/starterwarefree-code/third_party/fatfs/src/ff.c \
/home/thanasis/projects/starterwarefree-code/system_config/armv7a/mmu.c 

OBJS += \
./bl_copy.obj \
./bl_hsmmcsd.obj \
./bl_init.obj \
./bl_main.obj \
./bl_platform.obj \
./cache.obj \
./cp15.obj \
./device.obj \
./fat_mmcsd.obj \
./ff.obj \
./mmu.obj 

ASM_DEPS += \
./bl_init.pp \
./cp15.pp 

C_DEPS += \
./bl_copy.pp \
./bl_hsmmcsd.pp \
./bl_main.pp \
./bl_platform.pp \
./cache.pp \
./device.pp \
./fat_mmcsd.pp \
./ff.pp \
./mmu.pp 

C_DEPS__QUOTED += \
"bl_copy.pp" \
"bl_hsmmcsd.pp" \
"bl_main.pp" \
"bl_platform.pp" \
"cache.pp" \
"device.pp" \
"fat_mmcsd.pp" \
"ff.pp" \
"mmu.pp" 

OBJS__QUOTED += \
"bl_copy.obj" \
"bl_hsmmcsd.obj" \
"bl_init.obj" \
"bl_main.obj" \
"bl_platform.obj" \
"cache.obj" \
"cp15.obj" \
"device.obj" \
"fat_mmcsd.obj" \
"ff.obj" \
"mmu.obj" 

ASM_DEPS__QUOTED += \
"bl_init.pp" \
"cp15.pp" 

C_SRCS__QUOTED += \
"/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_copy.c" \
"/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_hsmmcsd.c" \
"/home/thanasis/projects/starterwarefree-code/bootloader/src/bl_main.c" \
"/home/thanasis/projects/starterwarefree-code/bootloader/src/armv7a/am335x/bl_platform.c" \
"/home/thanasis/projects/starterwarefree-code/system_config/armv7a/cache.c" \
"/home/thanasis/projects/starterwarefree-code/system_config/armv7a/am335x/device.c" \
"/home/thanasis/projects/starterwarefree-code/third_party/fatfs/port/fat_mmcsd.c" \
"/home/thanasis/projects/starterwarefree-code/third_party/fatfs/src/ff.c" \
"/home/thanasis/projects/starterwarefree-code/system_config/armv7a/mmu.c" 

ASM_SRCS__QUOTED += \
"/home/thanasis/projects/starterwarefree-code/bootloader/src/armv7a/cgt/bl_init.asm" \
"/home/thanasis/projects/starterwarefree-code/system_config/armv7a/cgt/cp15.asm" 


