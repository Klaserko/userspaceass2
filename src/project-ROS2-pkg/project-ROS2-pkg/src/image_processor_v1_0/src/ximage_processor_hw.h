// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// AXI_CPU
// 0x000 : Control signals
//         bit 0  - ap_start (Read/Write/COH)
//         bit 1  - ap_done (Read/COR)
//         bit 2  - ap_idle (Read)
//         bit 3  - ap_ready (Read)
//         bit 7  - auto_restart (Read/Write)
//         others - reserved
// 0x004 : Global Interrupt Enable Register
//         bit 0  - Global Interrupt Enable (Read/Write)
//         others - reserved
// 0x008 : IP Interrupt Enable Register (Read/Write)
//         bit 0  - enable ap_done interrupt (Read/Write)
//         bit 1  - enable ap_ready interrupt (Read/Write)
//         others - reserved
// 0x00c : IP Interrupt Status Register (Read/TOW)
//         bit 0  - ap_done (COR/TOW)
//         bit 1  - ap_ready (COR/TOW)
//         others - reserved
// 0x400 ~
// 0x7ff : Memory 'img_in' (256 * 32b)
//         Word n : bit [31:0] - img_in[n]
// 0x800 ~
// 0xbff : Memory 'out_r' (256 * 32b)
//         Word n : bit [31:0] - out_r[n]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL     0x000
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_GIE         0x004
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_IER         0x008
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_ISR         0x00c
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_BASE 0x400
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_IMG_IN_HIGH 0x7ff
#define XIMAGE_PROCESSOR_AXI_CPU_WIDTH_IMG_IN     32
#define XIMAGE_PROCESSOR_AXI_CPU_DEPTH_IMG_IN     256
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_BASE  0x800
#define XIMAGE_PROCESSOR_AXI_CPU_ADDR_OUT_R_HIGH  0xbff
#define XIMAGE_PROCESSOR_AXI_CPU_WIDTH_OUT_R      32
#define XIMAGE_PROCESSOR_AXI_CPU_DEPTH_OUT_R      256

