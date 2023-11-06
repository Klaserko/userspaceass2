// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "ximage_processor.h"

extern XImage_processor_Config XImage_processor_ConfigTable[];

XImage_processor_Config *XImage_processor_LookupConfig(u16 DeviceId) {
	XImage_processor_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XIMAGE_PROCESSOR_NUM_INSTANCES; Index++) {
		if (XImage_processor_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XImage_processor_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XImage_processor_Initialize(XImage_processor *InstancePtr, u16 DeviceId) {
	XImage_processor_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XImage_processor_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XImage_processor_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

