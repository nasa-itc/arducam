/*******************************************************************************
** File:
**  arducam_app_msg.h
**
** Purpose:
**  Define Arducam App Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _ARDUCAM_APP_MSG_H_
#define _ARDUCAM_APP_MSG_H_

#include "osapi.h" // for types used below
#include "cfe_sb.h" // for CFE_SB_CMD_HDR_SIZE, CFE_SB_TLM_HDR_SIZE
#include "cam_lib.h" // for other stuff like CAM_DATA_SIZE


/*
** ARDUCAM App command codes
*/
#define ARDUCAM_APP_NOOP_CC            0
#define ARDUCAM_APP_RESET_COUNTERS_CC  1
#define ARDUCAM_GET_DEV_DATA_CC        2
#define ARDUCAM_CONFIG_CC              3
#define ARDUCAM_OTHER_CMD_CC           4
#define ARDUCAM_RAW_CMD_CC             5
#define ARDUCAM_APP_RESET_DEV_CNTRS_CC 6
#define ARDUCAM_SEND_DEV_HK_CC         7
#define ARDUCAM_SEND_DEV_DATA_CC       8

/*
** Added from the previous arducam
*/

/* Generic Science CC */
// \camcmd CAM Stop Science
#define CAM_STOP_CC		        	2		// Stop all science in preperation for immediate shutdown
// \camcmd CAM Pause Science
#define CAM_PAUSE_CC				3		// Pause data transfer
// \camcmd CAM Resume Science 
#define CAM_RESUME_CC				4		// Resume data transfer
// \camcmd CAM Timeout Science      
#define CAM_TIMEOUT_CC              5       // Stop all science due to experiment timeout
// \camcmd CAM Low Voltage
#define CAM_LOW_VOLTAGE_CC          6       // Stop all science due to low voltage

/* Complete Experiment CC */
// \camcmd CAM Experiment 1 - Small
#define CAM_EXP1_CC					10     
// \camcmd CAM Experiment 2 - Medium
#define CAM_EXP2_CC			     	11       
// \camcmd CAM Experiment 3 - Large
#define CAM_EXP3_CC			     	12   
// \camcmd CAM Hardware Check
#define CAM_HW_CHECK_CC             13

/* Debug and Testing CC */
#define CAM_HWLIB_INIT_I2C_CC       20
#define CAM_HWLIB_INIT_SPI_CC       21
#define CAM_HWLIB_CONFIG_CC         22
#define CAM_HWLIB_JPEG_INIT_CC      23
#define CAM_HWLIB_YUV422_CC         24
#define CAM_HWLIB_JPEG_CC           25
#define CAM_HWLIB_SETUP_CC          26
#define CAM_HWLIB_SETSIZE_CC        27
#define CAM_HWLIB_CAPTURE_PREP_CC   28
#define CAM_HWLIB_CAPTURE_CC        29
#define CAM_HWLIB_READ_PREP_CC      30
#define CAM_HWLIB_READ_CC           31
#define CAM_PUBLISH_CC              32

/* Manager Application CC */
#define CAM_MGR_EOE_1_SUCCESS_CC    50
#define CAM_MGR_EOE_1_FAILURE_CC    51
#define CAM_MGR_EOE_2_SUCCESS_CC    52
#define CAM_MGR_EOE_2_FAILURE_CC    53
#define CAM_MGR_EOE_3_SUCCESS_CC    54
#define CAM_MGR_EOE_3_FAILURE_CC    55

/* PR Application CC */
#define CAM_PR_PAUSE_CC             0
#define CAM_PR_RESUME_CC            1


/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];

} ARDUCAM_NoArgsCmd_t;
#define CAM_NOARGSCMD_LNGTH sizeof (ARDUCAM_NoArgsCmd_t)

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which_open_mode
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_Noop_t;
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_ResetCounters_t;
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_Process_t;

typedef ARDUCAM_NoArgsCmd_t ARDUCAM_GetDevData_cmd_t;
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_Other_cmd_t;
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_SendDevHk_cmd_t;
typedef ARDUCAM_NoArgsCmd_t ARDUCAM_SendDevData_cmd_t;

/*
** ARDUCAM write configuration command
*/
typedef struct
{
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32   MillisecondStreamDelay;

} ARDUCAM_Config_cmd_t;

/*
** ARDUCAM raw command
*/
typedef struct
{
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8    RawCmd[5];
} ARDUCAM_Raw_cmd_t;

/*************************************************************************/
/*
** Type definition (ARDUCAM App housekeeping)
*/

typedef struct
{
    uint8 CommandErrorCounter;
    uint8 CommandCounter;
} OS_PACK ARDUCAM_HkTlm_Payload_t;
#define ARDUCAM_HK_TLM_PYLD sizeof ( ARDUCAM_HkTlm_Payload_t )

typedef struct
{
    uint8                  TlmHeader[CFE_SB_TLM_HDR_SIZE];
    ARDUCAM_HkTlm_Payload_t Payload;

} OS_PACK ARDUCAM_HkTlm_t;
#define ARDUCAM_HK_TLM_LNGTH sizeof ( ARDUCAM_HkTlm_t )

/*
** Type definition (ARDUCAM EXP)
** \camtlm ARDUCAM Experiment telemetry packet
** #ARDUCAM_EXP_TLM_MID
*/
typedef struct 
{
    uint8       TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8		data[CAM_DATA_SIZE];
    uint32		msg_count;
    uint32      length;
    
} ARDUCAM_Exp_tlm_t;
#define ARDUCAM_EXP_TLM_LNGTH  sizeof ( ARDUCAM_Exp_tlm_t )

#endif /* _ARDUCAM_APP_MSG_H_ */

/************************/
/*  End of File Comment */
/************************/
