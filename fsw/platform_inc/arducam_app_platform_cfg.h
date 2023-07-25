/************************************************************************
** File:
**  arducam_app_platform_cfg.h
**
** Purpose: 
**  Define Arducam App platform configuration parameters
**
** Notes:
**
**
*************************************************************************/
#ifndef _ARDUCAM_APP_PLATFORM_CFG_H_
#define _ARDUCAM_APP_PLATFORM_CFG_H_

#define ARDUCAM_UART_BUFFER_SIZE            1024

#define ARDUCAM_DEVICE_NAME                 "ARDUCAM_DEVICE"
/* Note: Arducam child task stack and priority get defined here. Main app is specified in cfe_es_startup.scr */
#define ARDUCAM_DEVICE_CHILD_STACK_SIZE     2048
#define ARDUCAM_DEVICE_CHILD_PRIORITY       100
#define ARDUCAM_DEVICE_MUTEX_NAME           "ARDUCAM_DEVICE_MUTEX"

/*
** Added from previous ARDUCAM
*/
#define ARDUCAM_CHILD_TASK_NAME             "ARDUCAM_CHILD_TASK"
#define ARDUCAM_RUN                         0
#define ARDUCAM_PAUSE                       1
#define ARDUCAM_STOP                        2
#define ARDUCAM_TIME                        3
#define ARDUCAM_LOW_VOLTAGE                 4

#define ARDUCAM_SEM_NAME                    "ARDUCAM_SEM"

/****************************************************/
/* Sensor related definition 						*/
/****************************************************/
#define BMP     0
#define JPEG    1

#define OV2640_160x120 		0	//160x120
#define OV2640_176x144 		1	//176x144
#define OV2640_320x240 		2	//320x240
#define OV2640_352x288 		3	//352x288
#define OV2640_640x480		4	//640x480
#define OV2640_800x600 		5	//800x600
#define OV2640_1024x768		6	//1024x768
#define OV2640_1280x1024	7	//1280x1024
#define OV2640_1600x1200	8	//1600x1200

#define ARDUCHIP_MODE       0x02 //Mode register

/*
** Default Arducam Configuration
** Note: In practice this configuration should be specified in the mission_defs/cpuX_device_cfg.h
**       These parameters are for the I/O to the device and can change per platform (NOS3 vs. flight vs. flatsat)
**       The definitions here will only be used if ARDUCAM_CFG is not in the cpuX_device_cfg.h 
*/
#ifndef ARDUCAM_CFG
    /* Note: NOS3 uart requires matching handle and bus number */
    #define ARDUCAM_CFG_STRING           "usart_29"
    #define ARDUCAM_CFG_HANDLE           29 
    #define ARDUCAM_CFG_BAUDRATE_HZ      115200
    #define ARDUCAM_CFG_MS_TIMEOUT       250
#endif

#endif /* _ARDUCAM_PLATFORM_CFG_H_ */

/************************/
/*  End of File Comment */
/************************/
