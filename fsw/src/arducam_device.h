/*******************************************************************************
** File:
**  arducam_device.h
**
** Purpose:
**   This file is the header file for the Arducam device
**
**
*******************************************************************************/

#ifndef _ARDUCAM_DEVICE_H_
#define _ARDUCAM_DEVICE_H_

#include "arducam_device_msg.h"

/*
 * Buffers to hold telemetry data prior to sending
 * Defined as a union to ensure proper alignment for a CFE_SB_Msg_t type
 */

typedef union
{
    CFE_SB_Msg_t         MsgHdr;
    ARDUCAM_DeviceHkTlm_t HkTlm;
} ARDUCAM_DeviceHkBuffer_t;

typedef union
{
    CFE_SB_Msg_t             MsgHdr;
    ARDUCAM_DeviceArducamTlm_t ArducamTlm;
} ARDUCAM_DeviceArducamBuffer_t;

/*
** Run Status variable used in the main processing loop.  If the device is asynchronous, this Status
** variable is also used in the device child processing loop.
*/
extern uint32 RunStatus;

/****************************************************************************/
/*
** Function prototypes.
**
*/
int32 ARDUCAM_DeviceInit(void);
int32 ARDUCAM_DeviceShutdown(void);
void  ARDUCAM_DeviceResetCounters(void);

void  ARDUCAM_DeviceGetArducamDataCommand(void);
void  ARDUCAM_DeviceConfigurationCommand(uint32_t millisecond_stream_delay);
void  ARDUCAM_DeviceOtherCommand(void);
void  ARDUCAM_DeviceRawCommand(const uint8 cmd[], const uint32_t cmd_length);

void ARDUCAM_ReportDeviceHousekeeping(void);
void ARDUCAM_ReportDeviceArducamData(void);

#endif

/************************/
/*  End of File Comment */
/************************/
