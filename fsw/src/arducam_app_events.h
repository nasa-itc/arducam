/************************************************************************
** File:
**  arducam_app_events.h
**
** Purpose:
**  Define Arducam App Event IDs
**
** Notes:
**
*************************************************************************/
#ifndef _ARDUCAM_APP_EVENTS_H_
#define _ARDUCAM_APP_EVENTS_H_

#define ARDUCAM_RESERVED_EID           0
#define ARDUCAM_STARTUP_INF_EID        1
#define ARDUCAM_COMMAND_ERR_EID        2
#define ARDUCAM_COMMANDNOP_INF_EID     3
#define ARDUCAM_COMMANDRST_INF_EID     4
#define ARDUCAM_INVALID_MSGID_ERR_EID  5
#define ARDUCAM_LEN_ERR_EID            6
#define ARDUCAM_PIPE_ERR_EID           7
#define ARDUCAM_CMD_DEVRST_INF_EID     8
#define ARDUCAM_UART_ERR_EID           9
#define ARDUCAM_UART_WRITE_ERR_EID    10
#define ARDUCAM_UART_READ_ERR_EID     11
#define ARDUCAM_COMMANDRAW_INF_EID    12
#define ARDUCAM_UART_MSG_CNT_DBG_EID  13
#define ARDUCAM_MUTEX_ERR_EID         14
#define ARDUCAM_CREATE_DEVICE_ERR_EID 15
#define ARDUCAM_DEVICE_REG_ERR_EID    16
#define ARDUCAM_DEVICE_REG_INF_EID    17

#define ARDUCAM_EVENT_COUNTS 17

/*
** The following are from the old arducam, but I think they might be unnecessary.
#define CAM_SEMAPHORE_ERR_EID         9
#define CAM_CHILD_REG_ERR_EID         10
#define CAM_INIT_CHILD_ERR_EID        11
#define CAM_INIT_ERR_EID              12
#define CAM_INIT_REG_ERR_EID          13
#define CAM_INIT_PIPE_ERR_EID         14
#define CAM_INIT_SUB_CMD_ERR_EID      15
#define CAM_INIT_SUB_HK_ERR_EID       16

* Child Task IDs *
#define CAM_STOP_INF_EID			  20
#define CAM_PAUSE_INF_EID			  21
#define CAM_RUN_INF_EID			   	  22
#define CAM_TIMEOUT_INF_EID           23  
#define CAM_LOW_VOLTAGE_INT_EID       24
#define CAM_CHILD_STOP_INF_EID	   	  25
#define CAM_CHILD_PAUSE_INF_EID	   	  26
#define CAM_CHILD_RUN_INF_EID	   	  27
#define CAM_CHILD_INIT_EID			  28
#define CAM_CHILD_INIT_ERR_EID		  29
#define CAM_CHILD_EXP_EID			  30
#define CAM_CHILD_EXP_ERR_EID		  31

* Full Experiments Completed *
#define CAM_EXP1_EID			      40
#define CAM_EXP2_EID			      41
#define CAM_EXP3_EID			      42
#define CAM_HW_CHECK_EID              43 

* Errors *
#define CAM_INIT_SPI_ERR_EID          61
#define CAM_INIT_I2C_ERR_EID          62
#define CAM_CONFIG_ERR_EID            63
#define CAM_JPEG_INIT_ERR_EID         64
#define CAM_YUV422_ERR_EID            65
#define CAM_JPEG_ERR_EID              66
#define CAM_SETUP_ERR_EID             67
#define CAM_SET_SIZE_ERR_EID          68
#define CAM_CAPTURE_PREP_ERR_EID      69
#define CAM_CAPTURE_ERR_EID           70
#define CAM_READ_FIFO_LEN_ERR_EID     71
#define CAM_READ_PREP_ERR_EID         72
#define CAM_READ_ERR_EID              73
#define CAM_PUBLISH_ERR_EID           74
#define CAM_LOW_VOLTAGE_EID           75
#define CAM_TIME_EID                  76

*/
#define CAM_INIT_REG_ERR_EID          13
#define CAM_INIT_PIPE_ERR_EID         14
#define CAM_INIT_SUB_CMD_ERR_EID      15
#define CAM_INIT_SUB_HK_ERR_EID       16
#define CAM_INIT_REG_ERR_EID          13
#define CAM_INIT_PIPE_ERR_EID         14
#define CAM_INIT_SUB_CMD_ERR_EID      15
#define CAM_INIT_SUB_HK_ERR_EID       16

#endif /* _ARDUCAM_APP_EVENTS_H_ */

/************************/
/*  End of File Comment */
/************************/
