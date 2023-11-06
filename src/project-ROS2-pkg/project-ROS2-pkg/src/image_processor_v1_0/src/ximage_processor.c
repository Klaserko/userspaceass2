// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "ximage_processor.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XImage_processor_CfgInitialize(XImage_processor *InstancePtr, XImage_processor_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axi_cpu_BaseAddress = ConfigPtr->Axi_cpu_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XImage_processor_Start(XImage_processor *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL) & 0x80;
    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL, Data | 0x01);
}

u32 XImage_processor_IsDone(XImage_processor *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XImage_processor_IsIdle(XImage_processor *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XImage_processor_IsReady(XImage_processor *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XImage_processor_EnableAutoRestart(XImage_processor *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL, 0x80);
}

void XImage_processor_DisableAutoRestart(XImage_processor *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL, 0);
}

u32 XImage_processor_Get_img_in_BaseAddress(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE);
}

u32 XImage_processor_Get_img_in_HighAddress(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH);
}

u32 XImage_processor_Get_img_in_TotalBytes(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + 1);
}

u32 XImage_processor_Get_img_in_BitWidth(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XIMAGE_PROCESSOR_AXI_CPU_WIDTH_IMG_IN;
}

u32 XImage_processor_Get_img_in_Depth(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XIMAGE_PROCESSOR_AXI_CPU_DEPTH_IMG_IN;
}

u32 XImage_processor_Write_img_in_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XImage_processor_Read_img_in_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + (offset + i)*4);
    }
    return length;
}

u32 XImage_processor_Write_img_in_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XImage_processor_Read_img_in_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE + offset + i);
    }
    return length;
}

u32 XImage_processor_Get_out_r_BaseAddress(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE);
}

u32 XImage_processor_Get_out_r_HighAddress(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH);
}

u32 XImage_processor_Get_out_r_TotalBytes(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + 1);
}

u32 XImage_processor_Get_out_r_BitWidth(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XIMAGE_PROCESSOR_AXI_CPU_WIDTH_OUT_R;
}

u32 XImage_processor_Get_out_r_Depth(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XIMAGE_PROCESSOR_AXI_CPU_DEPTH_OUT_R;
}

u32 XImage_processor_Write_out_r_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XImage_processor_Read_out_r_Words(XImage_processor *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + (offset + i)*4);
    }
    return length;
}

u32 XImage_processor_Write_out_r_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XImage_processor_Read_out_r_Bytes(XImage_processor *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH - XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axi_cpu_BaseAddress + XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE + offset + i);
    }
    return length;
}

void XImage_processor_InterruptGlobalEnable(XImage_processor *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_GIE, 1);
}

void XImage_processor_InterruptGlobalDisable(XImage_processor *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_GIE, 0);
}

void XImage_processor_InterruptEnable(XImage_processor *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER);
    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER, Register | Mask);
}

void XImage_processor_InterruptDisable(XImage_processor *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER);
    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER, Register & (~Mask));
}

void XImage_processor_InterruptClear(XImage_processor *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_ISR, Mask);
}

u32 XImage_processor_InterruptGetEnabled(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER);
}

u32 XImage_processor_InterruptGetStatus(XImage_processor *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_ISR);
}

