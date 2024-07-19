// File Name: DD_COM_ParPolling.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev002 Base)
// Revision 000

#ifndef DD_COM_PAR_POLLING_C
#define DD_COM_PAR_POLLING_C

#include "Common.h"
#include "LIB_Service.h"
#include "DD_SCI.h"
#include "DD_COM_Commondef.h"
#include "DD_COM_ParPolling.h"
#include "DD_COM_Service.h"

// ◆ state
typedef enum{
	stIdle_ID,
	stReset_ID,
	stExecute_ID,
	stStop_ID,
}DD_COM_PollingStateID;

// ◆ Prototype(Used Only in DD_COM_Par_Polling.c)
static void stReset(void);
static void stExecute(void);
static void stStop(void);

// ◆ Grobal Parameter & Type define
static DD_COM_PollingStateID state = stIdle_ID;

UC POLLONG_CMD[] = { 0xF0 };

//==============================================================================
// Function Name : stReset
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stReset()
{
	state = stReset_ID;
	
	DD_EVT_SetTimer( DD_COM_POLLING_INTARVAL, evuDD_COM_ParPolling_ID, tmDD_COM_ParPolling_ID, 0);
}

//==============================================================================
// Function Name : stExecute
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stExecute()
{
	state = stExecute_ID;
	
	DD_COM_SendMsg(&POLLONG_CMD[0],1);
	DD_EVT_Put(evGoComplete_ID, evuDD_COM_ParPolling_ID, 0, 0);
}

//==============================================================================
// Function Name : stStop
// Description   : 
// Parameter     : なし
// Return        : なし
//==============================================================================
static void stStop()
{
	state = stStop_ID;
	
	DD_EVT_CancelTimer(tmDD_COM_ParPolling_ID);
}

//==============================================================================
// Function Name : DD_COM_ParPollingNotify
// Description   : 
// Parameter     : eventId :イベントID
//　　　　　　　　 arg:未使用
// Return        : なし
//==============================================================================
void DD_COM_ParPollingNotify(DD_EventID eventId, UC* pMsg)
{
	switch (eventId) {
	case evStart_ID:
		switch(state){
		case stIdle_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	case evReset_ID:
		switch(state){
		case stReset_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	case evTimeout_ID:
		switch(state){
		case stReset_ID:
			stExecute();
			break;
		default:
			break;
		}
		break;
	case evStop_ID:
		switch(state){
		case stReset_ID:
			stStop();
			break;
		default:
			break;
		}
		break;
	case evGoComplete_ID:
		switch(state){
		case stExecute_ID:
			stReset();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

#endif
	