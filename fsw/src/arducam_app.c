/*******************************************************************************
** File: 
**  arducam_app.c
**
** Purpose:
**   This file contains the source code for the Arducam App.
**
*******************************************************************************/

/*
** Include Files:
*/

//#include "string.h"
//#include "arducam_app_events.h"

#include "arducam_app.h"
#include "arducam_app_version.h"
#include "arducam_app_msgids.h"
#include "arducam_app_perfids.h"
#include "arducam_device.h"
#include "cfe_error.h"

/*
** global data
*/
ARDUCAM_AppData_t ARDUCAM_AppData;

// Forward declarations
static int32 ARDUCAM_AppInit(void);
static void  ARDUCAM_ProcessCommandPacket(CFE_SB_MsgPtr_t Msg);
static void  ARDUCAM_ProcessGroundCommand(CFE_SB_MsgPtr_t Msg);
static int32 ARDUCAM_ReportHousekeeping(const CFE_SB_CmdHdr_t *Msg);
static int32 ARDUCAM_ResetCounters(const ARDUCAM_ResetCounters_t *Msg);
static int32 ARDUCAM_Noop(const ARDUCAM_Noop_t *Msg);
static bool ARDUCAM_VerifyCmdLength(CFE_SB_MsgPtr_t Msg, uint16 ExpectedLength);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Name:  ARDUCAM_AppMain()                                                    */
/* Purpose:                                                                   */
/*        Application entry point and main process loop                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void ARDUCAM_AppMain(void)
{
    int32 status;

    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp();

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(ARDUCAM_APP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = ARDUCAM_AppInit();
    if (status != CFE_SUCCESS)
    {
        RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** ARDUCAM Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(ARDUCAM_APP_PERF_ID);

        /* Pend on receipt of command packet */
        status = CFE_SB_RcvMsg(&ARDUCAM_AppData.MsgPtr, ARDUCAM_AppData.CommandPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(ARDUCAM_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            ARDUCAM_ProcessCommandPacket(ARDUCAM_AppData.MsgPtr);
        }
        else
        {
            CFE_EVS_SendEvent(ARDUCAM_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "ARDUCAM APP: SB Pipe Read Error, App Will Exit");

            RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    RunStatus = CFE_ES_RunStatus_APP_EXIT; // we are wanting to exit... make sure everyone knows it

    status = ARDUCAM_DeviceShutdown();
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error Shutting Down Device, RC = 0x%08lX\n", (unsigned long)status);
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(ARDUCAM_APP_PERF_ID);

    CFE_ES_ExitApp(RunStatus);

} /* End of ARDUCAM_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Name:  ARDUCAM_AppInit()                                                    */
/*                                                                            */
/* Purpose:                                                                   */
/*        Initialization                                                      */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
static int32 ARDUCAM_AppInit(void)
{
    int32 status;

    RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandCounter = 0;
    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter = 0;

    /*
    ** Initialize app configuration data
    */
    ARDUCAM_AppData.PipeDepth = ARDUCAM_PIPE_DEPTH;

    /*
    ** Initialize event filter table...
    */
    ARDUCAM_AppData.EventFilters[0].EventID = ARDUCAM_STARTUP_INF_EID;
    ARDUCAM_AppData.EventFilters[0].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[1].EventID = ARDUCAM_COMMAND_ERR_EID;
    ARDUCAM_AppData.EventFilters[1].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[2].EventID = ARDUCAM_COMMANDNOP_INF_EID;
    ARDUCAM_AppData.EventFilters[2].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[3].EventID = ARDUCAM_COMMANDRST_INF_EID;
    ARDUCAM_AppData.EventFilters[3].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[4].EventID = ARDUCAM_INVALID_MSGID_ERR_EID;
    ARDUCAM_AppData.EventFilters[4].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[5].EventID = ARDUCAM_LEN_ERR_EID;
    ARDUCAM_AppData.EventFilters[5].Mask    = 0x0000;
    ARDUCAM_AppData.EventFilters[6].EventID = ARDUCAM_PIPE_ERR_EID;
    ARDUCAM_AppData.EventFilters[6].Mask    = 0x0000;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(ARDUCAM_AppData.EventFilters, ARDUCAM_EVENT_COUNTS, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_SB_InitMsg(&ARDUCAM_AppData.HkBuf.MsgHdr, ARDUCAM_APP_HK_TLM_MID, sizeof(ARDUCAM_AppData.HkBuf), true);

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&ARDUCAM_AppData.CommandPipe, ARDUCAM_AppData.PipeDepth, "ARDUCAM_CMD_PIPE");
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(ARDUCAM_APP_SEND_HK_MID, ARDUCAM_AppData.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return (status);
    }

    /*
    ** Subscribe to ground command packets
    */
    status = CFE_SB_Subscribe(ARDUCAM_APP_CMD_MID, ARDUCAM_AppData.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);

        return (status);
    }

    status = ARDUCAM_DeviceInit();
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Arducam App: Error Initializing Device, RC = 0x%08lX\n", (unsigned long)status);

        return (status);
    }

    CFE_EVS_SendEvent(ARDUCAM_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "ARDUCAM App Initialized. Version %d.%d.%d.%d",
                      ARDUCAM_APP_MAJOR_VERSION,
                      ARDUCAM_APP_MINOR_VERSION,
                      ARDUCAM_APP_REVISION,
                      ARDUCAM_APP_MISSION_REV);

    return (CFE_SUCCESS);

} /* End of ARDUCAM_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  ARDUCAM_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the ARDUCAM    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
static void ARDUCAM_ProcessCommandPacket(CFE_SB_MsgPtr_t Msg)
{
    CFE_SB_MsgId_t MsgId;

    MsgId = CFE_SB_GetMsgId(Msg);

    switch (MsgId)
    {
        case ARDUCAM_APP_CMD_MID:
            ARDUCAM_ProcessGroundCommand(Msg);
            break;

        case ARDUCAM_APP_SEND_HK_MID:
            ARDUCAM_ReportHousekeeping((CFE_SB_CmdHdr_t *)Msg);
            break;

        default:
            CFE_EVS_SendEvent(ARDUCAM_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "ARDUCAM: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }

    return;

} /* End ARDUCAM_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Name:  ARDUCAM_ProcessGroundCommand()                                       */
/*                                                                            */
/* Purpose:                                                                   */
/*        ARDUCAM ground commands                                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
static void ARDUCAM_ProcessGroundCommand(CFE_SB_MsgPtr_t Msg)
{
    uint16 CommandCode = CFE_SB_GetCmdCode(Msg);
    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandCounter++;

    /*
    ** Process "known" ARDUCAM app ground commands
    */
    switch (CommandCode)
    {
        case ARDUCAM_APP_NOOP_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_Noop_t)))
            {
                ARDUCAM_Noop((ARDUCAM_Noop_t *)Msg);
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }

            break;

        case ARDUCAM_APP_RESET_COUNTERS_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_ResetCounters_t)))
            {
                ARDUCAM_ResetCounters((ARDUCAM_ResetCounters_t *)Msg);
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }

            break;

        /*
        ** TODO: Edit and add more command codes as appropriate for the application
        */
        case ARDUCAM_APP_RESET_DEV_CNTRS_CC:
            ARDUCAM_DeviceResetCounters();
            break;

        case ARDUCAM_GET_DEV_DATA_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_GetDevData_cmd_t))) {
                ARDUCAM_DeviceGetArducamDataCommand();
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        case ARDUCAM_CONFIG_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_Config_cmd_t)))
            {
                ARDUCAM_DeviceConfigurationCommand(((ARDUCAM_Config_cmd_t *)Msg)->MillisecondStreamDelay);
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        case ARDUCAM_OTHER_CMD_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_Other_cmd_t)))
            {
                ARDUCAM_DeviceOtherCommand();
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        case ARDUCAM_RAW_CMD_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_Raw_cmd_t)))
            {
                ARDUCAM_DeviceRawCommand(((ARDUCAM_Raw_cmd_t *)Msg)->RawCmd, sizeof(((ARDUCAM_Raw_cmd_t *)Msg)->RawCmd));
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        case ARDUCAM_SEND_DEV_HK_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_SendDevHk_cmd_t))) {
                ARDUCAM_ReportDeviceHousekeeping();
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        case ARDUCAM_SEND_DEV_DATA_CC:
            if (ARDUCAM_VerifyCmdLength(Msg, sizeof(ARDUCAM_SendDevData_cmd_t))) {
                ARDUCAM_ReportDeviceArducamData();
            } else {
                ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            }
            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(ARDUCAM_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid ground command code: CC = %d",
                              CommandCode);
            ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
            break;
    }

    return;

} /* End of ARDUCAM_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  ARDUCAM_ReportHousekeeping                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
static int32 ARDUCAM_ReportHousekeeping(const CFE_SB_CmdHdr_t *Msg)
{
    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandCounter++;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(&ARDUCAM_AppData.HkBuf.MsgHdr);
    CFE_SB_SendMsg(&ARDUCAM_AppData.HkBuf.MsgHdr);

    return CFE_SUCCESS;

} /* End of ARDUCAM_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Name:  ARDUCAM_Noop                                                         */
/*                                                                            */
/* Purpose:                                                                   */
/*        ARDUCAM NOOP command                                                 */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
static int32 ARDUCAM_Noop(const ARDUCAM_Noop_t *Msg)
{

    CFE_EVS_SendEvent(ARDUCAM_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "ARDUCAM: NOOP command");

    return CFE_SUCCESS;

} /* End of ARDUCAM_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  ARDUCAM_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
static int32 ARDUCAM_ResetCounters(const ARDUCAM_ResetCounters_t *Msg)
{

    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandCounter = 0;
    ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter = 0;

    CFE_EVS_SendEvent(ARDUCAM_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "ARDUCAM: RESET command");

    return CFE_SUCCESS;

} /* End of ARDUCAM_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Name:  ARDUCAM_VerifyCmdLength()                                            */
/*                                                                            */
/* Purpose:                                                                   */
/*        Verify command packet length                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
static bool ARDUCAM_VerifyCmdLength(CFE_SB_MsgPtr_t Msg, uint16 ExpectedLength)
{
    bool result = true;

    uint16 ActualLength = CFE_SB_GetTotalMsgLength(Msg);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID   = CFE_SB_GetMsgId(Msg);
        uint16         CommandCode = CFE_SB_GetCmdCode(Msg);

        CFE_EVS_SendEvent(ARDUCAM_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
                          (unsigned int)CFE_SB_MsgIdToValue(MessageID), CommandCode, ActualLength, ExpectedLength);

        result = false;

        ARDUCAM_AppData.HkBuf.HkTlm.Payload.CommandErrorCounter++;
    }

    return (result);

} /* End of ARDUCAM_VerifyCmdLength() */
