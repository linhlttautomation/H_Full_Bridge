################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/cpu1/sdfm_filters_sync_cla/cpu01/2837x_RAM_SDFM_CLA_lnk_cpu1.cmd 

CLA_SRCS += \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/cpu1/sdfm_filters_sync_cla/cpu01/sdfm_filter_sync.cla 

ASM_SRCS += \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_usDelay.asm 

C_SRCS += \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_DefaultISR.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_Gpio.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_Ipc.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_PieCtrl.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_PieVect.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_SysCtrl.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_sdfm_drivers.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_struct.c \
C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/cpu1/sdfm_filters_sync_cla/cpu01/sdfm_filter_sync_cla_cpu01.c 

CLA_DEPS += \
./task.d 

C_DEPS += \
./F2837xD_DefaultISR.d \
./F2837xD_GlobalVariableDefs.d \
./F2837xD_Gpio.d \
./F2837xD_Ipc.d \
./F2837xD_PieCtrl.d \
./F2837xD_PieVect.d \
./F2837xD_SysCtrl.d \
./F2837xD_sdfm_drivers.d \
./F2837xD_struct.d \
./main.d 

OBJS += \
./F2837xD_CodeStartBranch.obj \
./F2837xD_DefaultISR.obj \
./F2837xD_GlobalVariableDefs.obj \
./F2837xD_Gpio.obj \
./F2837xD_Ipc.obj \
./F2837xD_PieCtrl.obj \
./F2837xD_PieVect.obj \
./F2837xD_SysCtrl.obj \
./F2837xD_sdfm_drivers.obj \
./F2837xD_struct.obj \
./F2837xD_usDelay.obj \
./sdfm_filter_sync_cla_cpu01.obj \
./sdfm_filter_sync.obj 

ASM_DEPS += \
./F2837xD_CodeStartBranch.d \
./F2837xD_usDelay.d 

OBJS__QUOTED += \
"F2837xD_CodeStartBranch.obj" \
"F2837xD_DefaultISR.obj" \
"F2837xD_GlobalVariableDefs.obj" \
"F2837xD_Gpio.obj" \
"F2837xD_Ipc.obj" \
"F2837xD_PieCtrl.obj" \
"F2837xD_PieVect.obj" \
"F2837xD_SysCtrl.obj" \
"F2837xD_sdfm_drivers.obj" \
"F2837xD_struct.obj" \
"F2837xD_usDelay.obj" \
"sdfm_filter_sync_cla_cpu01.obj" \
"sdfm_filter_sync.obj" 

C_DEPS__QUOTED += \
"F2837xD_DefaultISR.d" \
"F2837xD_GlobalVariableDefs.d" \
"F2837xD_Gpio.d" \
"F2837xD_Ipc.d" \
"F2837xD_PieCtrl.d" \
"F2837xD_PieVect.d" \
"F2837xD_SysCtrl.d" \
"F2837xD_sdfm_drivers.d" \
"F2837xD_struct.d" \
"main.d" 

CLA_DEPS__QUOTED += \
"task.d" 

ASM_DEPS__QUOTED += \
"F2837xD_CodeStartBranch.d" \
"F2837xD_usDelay.d" 

ASM_SRCS__QUOTED += \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_usDelay.asm" 

C_SRCS__QUOTED += \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_DefaultISR.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_Gpio.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_Ipc.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_PieCtrl.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_PieVect.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_SysCtrl.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_sdfm_drivers.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/common/source/F2837xD_struct.c" \
"C:/ti/c2000/C2000Ware_5_03_00_00/device_support/f2837xd/examples/cpu1/sdfm_filters_sync_cla/cpu01/sdfm_filter_sync_cla_cpu01.c" 


