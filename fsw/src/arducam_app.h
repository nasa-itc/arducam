/*******************************************************************************
** File: 
**  arducam_app.h
**
** Purpose:
**   This file is main header file for the Arducam application.
**
*******************************************************************************/
#ifndef _ARDUCAM_APP_H_
#define _ARDUCAM_APP_H_

/*
** Required header files.
*/
#include "arducam_app_msg.h"
#include "arducam_app_events.h"
#include "cfe_sb.h"
#include "cfe_evs.h"

/***********************************************************************/
#define ARDUCAM_PIPE_DEPTH 32 /* Depth of the Command Pipe for Application */

/************************************************************************
** Type Definitions
*************************************************************************/

/*
 * Buffer to hold telemetry data prior to sending
 * Defined as a union to ensure proper alignment for a CFE_SB_Msg_t type
 */
typedef union
{
    CFE_SB_Msg_t   MsgHdr;
    ARDUCAM_HkTlm_t HkTlm;
} ARDUCAM_HkBuffer_t;

/*
** Global Data
*/
typedef struct
{
    /*
    ** Housekeeping telemetry packet...
    */
    ARDUCAM_HkBuffer_t HkBuf;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t CommandPipe;
    CFE_SB_MsgPtr_t MsgPtr;

    /*
    ** Initialization data (not reported in housekeeping)...
    */
    char   PipeName[16];
    uint16 PipeDepth;

    CFE_EVS_BinFilter_t EventFilters[ARDUCAM_EVENT_COUNTS];

} ARDUCAM_AppData_t;

/****************************************************************************/
/*
** Function prototypes.
**
** Note: Except for the entry point (ARDUCAM_AppMain), these
**       functions are not called from any other source module.
*/
void  ARDUCAM_AppMain(void);

#endif /* _arducam_app_h_ */

/************************/
/*  End of File Comment */
/************************/
