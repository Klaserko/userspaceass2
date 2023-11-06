// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XIMAGE_PROCESSOR_H
#define XIMAGE_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "ximage_processor_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Axi_cpu_BaseAddress;
} XImage_processor_Config;
#endif

typedef struct {
    u64 Axi_cpu_BaseAddress;
    u32 IsReady;
} XImage_processor;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XImage_processor_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XImage_processor_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XImage_processor_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XImage_processor_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XImage_processor_Initialize(XImage_processor *InstancePtr, u16 DeviceId);
XImage_processor_Config* XImage_processor_LookupConfig(u16 DeviceId);
int XImage_processor_CfgInitialize(XImage_processor *InstancePtr, XImage_processor_Config *ConfigPtr);
#else
int XImage_processor_Initialize(XImage_processor *InstancePtr, const char* InstanceName);
int XImage_processor_Release(XImage_processor *InstancePtr);
#endif

void XImage_processor_Start(XImage_processor *InstancePtr);
u32 XImage_processor_IsDone(XImage_processor *InstancePtr);
u32 XImage_processor_IsIdle(XImage_processor *InstancePtr);
u32 XImage_processor_IsReady(XImage_processor *InstancePtr);
void XImage_processor_EnableAutoRestart(XImage_processor *InstancePtr);
void XImage_processor_DisableAutoRestart(XImage_processor *InstancePtr);

u32 XImage_processor_Get_img_in_BaseAddress(XImage_processor *InstancePtr);
u32 XImage_processor_Get_img_in_HighAddress(XImage_processor *InstancePtr);
u32 XImage_processor_Get_img_in_TotalBytes(XImage_processor *InstancePtr);
u32 XImage_processor_Get_img_in_BitWidth(XImage_processor *InstancePtr);
u32 XImage_processor_Get_img_in_Depth(XImage_processor *InstancePtr);
u32 XImage_processor_Write_img_in_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length);
u32 XImage_processor_Read_img_in_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length);
u32 XImage_processor_Write_img_in_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length);
u32 XImage_processor_Read_img_in_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length);
u32 XImage_processor_Get_out_r_BaseAddress(XImage_processor *InstancePtr);
u32 XImage_processor_Get_out_r_HighAddress(XImage_processor *InstancePtr);
u32 XImage_processor_Get_out_r_TotalBytes(XImage_processor *InstancePtr);
u32 XImage_processor_Get_out_r_BitWidth(XImage_processor *InstancePtr);
u32 XImage_processor_Get_out_r_Depth(XImage_processor *InstancePtr);
u32 XImage_processor_Write_out_r_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length);
u32 XImage_processor_Read_out_r_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length);
u32 XImage_processor_Write_out_r_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length);
u32 XImage_processor_Read_out_r_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length);

void XImage_processor_InterruptGlobalEnable(XImage_processor *InstancePtr);
void XImage_processor_InterruptGlobalDisable(XImage_processor *InstancePtr);
void XImage_processor_InterruptEnable(XImage_processor *InstancePtr, u32 Mask);
void XImage_processor_InterruptDisable(XImage_processor *InstancePtr, u32 Mask);
void XImage_processor_InterruptClear(XImage_processor *InstancePtr, u32 Mask);
u32 XImage_processor_InterruptGetEnabled(XImage_processor *InstancePtr);
u32 XImage_processor_InterruptGetStatus(XImage_processor *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
