#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

#include <cv_bridge/cv_bridge.h>

#include <stdio.h>

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

// From _hw
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

// from_ximage_processor.c

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

void XImage_processor_DisableAutoRestart(XImage_processor *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XImage_processor_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL, 0);
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



#ifdef __linux__

/***************************** Include Files *********************************/
//#include "ximage_processor.h"

/***************** Macros (Inline Functions) Definitions *********************/
#define MAX_UIO_PATH_SIZE       256
#define MAX_UIO_NAME_SIZE       64
#define MAX_UIO_MAPS            5
#define UIO_INVALID_ADDR        0

/**************************** Type Definitions ******************************/
typedef struct {
    u64 addr;
    u32 size;
} XImage_processor_uio_map;

typedef struct {
    int  uio_fd;
    int  uio_num;
    char name[ MAX_UIO_NAME_SIZE ];
    char version[ MAX_UIO_NAME_SIZE ];
    XImage_processor_uio_map maps[ MAX_UIO_MAPS ];
} XImage_processor_uio_info;

/***************** Variable Definitions **************************************/
static XImage_processor_uio_info uio_info;

/************************** Function Implementation *************************/
static int line_from_file(char* filename, char* linebuf) {
    char* s;
    int i;
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    s = fgets(linebuf, MAX_UIO_NAME_SIZE, fp);
    fclose(fp);
    if (!s) return -2;
    for (i=0; (*s)&&(i<MAX_UIO_NAME_SIZE); i++) {
        if (*s == '\n') *s = 0;
        s++;
    }
    return 0;
}

static int uio_info_read_name(XImage_processor_uio_info* info) {
    char file[ MAX_UIO_PATH_SIZE ];
    sprintf(file, "/sys/class/uio/uio%d/name", info->uio_num);
    return line_from_file(file, info->name);
}

static int uio_info_read_version(XImage_processor_uio_info* info) {
    char file[ MAX_UIO_PATH_SIZE ];
    sprintf(file, "/sys/class/uio/uio%d/version", info->uio_num);
    return line_from_file(file, info->version);
}

static int uio_info_read_map_addr(XImage_processor_uio_info* info, int n) {
    int ret;
    char file[ MAX_UIO_PATH_SIZE ];
    info->maps[n].addr = UIO_INVALID_ADDR;
    sprintf(file, "/sys/class/uio/uio%d/maps/map%d/addr", info->uio_num, n);
    FILE* fp = fopen(file, "r");
    if (!fp) return -1;
    ret = fscanf(fp, "0x%x", &info->maps[n].addr);
    fclose(fp);
    if (ret < 0) return -2;
    return 0;
}

static int uio_info_read_map_size(XImage_processor_uio_info* info, int n) {
    int ret;
    char file[ MAX_UIO_PATH_SIZE ];
    sprintf(file, "/sys/class/uio/uio%d/maps/map%d/size", info->uio_num, n);
    FILE* fp = fopen(file, "r");
    if (!fp) return -1;
    ret = fscanf(fp, "0x%x", &info->maps[n].size);
    fclose(fp);
    if (ret < 0) return -2;
    return 0;
}

u32 XImage_processor_IsReady(XImage_processor *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XImage_processor_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XIMAGE_PROCESSOR_AXI_CPU_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

int XImage_processor_Initialize(XImage_processor *InstancePtr, const char* InstanceName) {
	XImage_processor_uio_info *InfoPtr = &uio_info;
	struct dirent **namelist;
    int i, n;
    char* s;
    char file[ MAX_UIO_PATH_SIZE ];
    char name[ MAX_UIO_NAME_SIZE ];
    int flag = 0;

    assert(InstancePtr != NULL);

    n = scandir("/sys/class/uio", &namelist, 0, alphasort);
    if (n < 0)  return XST_DEVICE_NOT_FOUND;
    for (i = 0;  i < n; i++) {
    	strcpy(file, "/sys/class/uio/");
    	strcat(file, namelist[i]->d_name);
    	strcat(file, "/name");
        if ((line_from_file(file, name) == 0) && (strcmp(name, InstanceName) == 0)) {
            flag = 1;
            s = namelist[i]->d_name;
            s += 3; // "uio"
            InfoPtr->uio_num = atoi(s);
            break;
        }
    }
    if (flag == 0)  return XST_DEVICE_NOT_FOUND;

    uio_info_read_name(InfoPtr);
    uio_info_read_version(InfoPtr);
    for (n = 0; n < MAX_UIO_MAPS; ++n) {
        uio_info_read_map_addr(InfoPtr, n);
        uio_info_read_map_size(InfoPtr, n);
    }

    sprintf(file, "/dev/uio%d", InfoPtr->uio_num);
    if ((InfoPtr->uio_fd = open(file, O_RDWR)) < 0) {
        return XST_OPEN_DEVICE_FAILED;
    }

    // NOTE: slave interface 'Axi_cpu' should be mapped to uioX/map0
    InstancePtr->Axi_cpu_BaseAddress = (u64)mmap(NULL, InfoPtr->maps[0].size, PROT_READ|PROT_WRITE, MAP_SHARED, InfoPtr->uio_fd, 0 * getpagesize());
    assert(InstancePtr->Axi_cpu_BaseAddress);

    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}

int XImage_processor_Release(XImage_processor *InstancePtr) {
	XImage_processor_uio_info *InfoPtr = &uio_info;

    assert(InstancePtr != NULL);
    assert(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    munmap((void*)InstancePtr->Axi_cpu_BaseAddress, InfoPtr->maps[0].size);

    close(InfoPtr->uio_fd);

    return XST_SUCCESS;
}

#endif


XImage_processor ip_inst;



class ImageSubscriber : public rclcpp::Node
{
	public:
		ImageSubscriber() : Node("image_subscriber") {
			RCLCPP_INFO(this->get_logger(), "Initializing ImageSubscriber node");

			RCLCPP_INFO(this->get_logger(), "Starting camera subscription");

			camera_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
					"/image_raw",
					10,
					std::bind(&ImageSubscriber::onImageMsg, this, std::placeholders::_1)
			);

		}

	private:
		rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_subscription_;

		void onImageMsg(const sensor_msgs::msg::Image::SharedPtr msg) {
			RCLCPP_INFO(this->get_logger(), "Received image!");

			cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, msg->encoding);
			cv::Mat img = cv_ptr->image;

			// Resize image
			unsigned int in1[16*16];
			unsigned int out[16*16];

			int cnt = 0;
			for(int i = 0; i<img.rows; i+= 30){
				for(int j = 0; i<img.cols; i+= 40){
				 	cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);	
					
					uchar blue = pixel[0];
        				uchar green = pixel[1];
        				uchar red = pixel[2];

					in1[cnt] = 0;
					in1[cnt] |= static_cast<uint32_t>(red) << 16;
					in1[cnt] |= static_cast<uint32_t>(green) << 8;
					in1[cnt] |= static_cast<uint32_t>(blue);

                    in1[cnt] &= 0x00FFFFFF;
 
					out[cnt] = 0;
					cnt++;
				}
			}

			char buffer_1[50];
			sprintf(buffer_1, "%x, %x, %x, %x, %x", in1[0], in1[1], in1[2], in1[3], in1[4]);
			
			RCLCPP_INFO(this->get_logger(), buffer_1);

            while(!ip_inst.IsReady);

			// Send data
			XImage_processor_Write_img_in_Words(&ip_inst, 0, in1, 16*16);

			// Start the IP
			XImage_processor_Start(&ip_inst);

			// Wait for the results
			while (!XImage_processor_IsDone(&ip_inst));

			// Read the output
			XImage_processor_Read_out_r_Words(&ip_inst, 0, out, 16*16);
			
			char buffer[50]; 
			sprintf(buffer, "%x %x %x %x %x", out[0], out[1], out[2], out[3], out[4]);

			RCLCPP_INFO(this->get_logger(), buffer);

			RCLCPP_INFO(this->get_logger(), "Successfully loaded image");


		}

};


int main(int argc, char *argv[])
{
	setvbuf(stdout,NULL,_IONBF,BUFSIZ);

	XImage_processor_Initialize(&ip_inst, "image_processor");

	rclcpp::init(argc,argv);
	rclcpp::spin(std::make_shared<ImageSubscriber>());

	XImage_processor_DisableAutoRestart(&ip_inst);

	rclcpp::shutdown();
	return 0;
}



