/*******************************************************************************
** File:
**  arducam_device_msg.h
**
** Purpose:
**  Define Arducam Device Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _ARDUCAM_DEVICE_MSG_H_
#define _ARDUCAM_DEVICE_MSG_H_

#include "osapi.h" // for types used below
#include "cfe_sb.h" // for CFE_SB_TLM_HDR_SIZE

/*************************************************************************/
/*
** Type definition (ARDUCAM Device housekeeping)
*/

typedef struct
{
    uint8 CommandErrorCounter;
    uint8 GetDataCmdCounter;
    uint8 CfgCmdCounter;
    uint8 OtherCmdCounter;
    uint8 RawCmdCounter;
} OS_PACK ARDUCAM_DeviceCmdData_t;
#define ARDUCAM_DEVICE_CMD_LNGTH sizeof (ARDUCAM_DeviceCmdData_t)

typedef struct
{
    uint8 CfgRespCounter;
    uint8 OtherRespCounter;
    uint8 RawRespCounter;
    uint32 UnknownResponseCounter;
    uint32 DeviceArducamDataCounter;
    uint32 MillisecondStreamDelay;
} OS_PACK ARDUCAM_DeviceRespHkData_t;
#define ARDUCAM_DEVICE_HK_DATA_LNGTH sizeof (ARDUCAM_DeviceRespHkData_t)

typedef struct
{
    ARDUCAM_DeviceCmdData_t    ARDUCAM_DeviceCmdData;
    ARDUCAM_DeviceRespHkData_t ARDUCAM_DeviceRespHkData;
} OS_PACK ARDUCAM_DeviceHkTlm_Payload_t;
#define ARDUCAM_DEVICE_HK_PAYLOAD_LNGTH sizeof (ARDUCAM_DeviceHkTlm_Payload_t)

typedef struct
{
    uint8                        TlmHeader[CFE_SB_TLM_HDR_SIZE];
    ARDUCAM_DeviceHkTlm_Payload_t Payload;

} OS_PACK ARDUCAM_DeviceHkTlm_t;
#define ARDUCAM_DEVICE_HKTLM_DATA_LNGTH sizeof (ARDUCAM_DeviceHkTlm_t)

/*************************************************************************/
/*
** Type definition (ARDUCAM Device Data)
*/

typedef struct
{
    uint32 ArducamProcessedTimeSeconds;
    uint32 ArducamProcessedTimeSubseconds;
    uint32 ArducamsSent;
    uint16 ArducamDataX;
    uint16 ArducamDataY;
    uint16 ArducamDataZ;
} OS_PACK ARDUCAM_DeviceRespArducamData_t;
#define ARDUCAM_DEVICE_RESP_DATA_LNGTH sizeof (ARDUCAM_DeviceRespArducamData_t)

typedef struct
{
    ARDUCAM_DeviceRespArducamData_t ARDUCAM_DeviceRespArducamData;
} OS_PACK ARDUCAM_ArducamTlm_Payload_t;
#define ARDUCAM_TLM_PAYLOAD_LNGTH sizeof (ARDUCAM_ArducamTlm_Payload_t)

typedef struct
{
    uint8                      TlmHeader[CFE_SB_TLM_HDR_SIZE];
    ARDUCAM_ArducamTlm_Payload_t Payload;

} OS_PACK ARDUCAM_DeviceArducamTlm_t;
#define ARDUCAM_DEVICE_TLM_LNGTH sizeof (ARDUCAM_DeviceArducamTlm_t)

#endif /* _ARDUCAM_DEVICE_MSG_H_ */

/************************/
/*  End of File Comment */
/************************/
