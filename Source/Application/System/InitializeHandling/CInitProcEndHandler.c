// File Name:	CInitProcEndHandler.c
// Description:	Implimentation File of Process End Action Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "CInitProcEndHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Process End Action Handler ---
typedef struct /*CInitProcEndHandlerMember*/
{
	const InitProcEndHandlerSpec* mpSpec;					// Pointer of Spec Data
	UC mState;												// Device Initialize State
} CInitProcEndHandlerMember;

static CInitProcEndHandlerMember gInit_ProcEndHandlerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_Constructor
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitProcEndHandler_Constructor(const InitProcEndHandlerSpec* pSpec)
{
	gInit_ProcEndHandlerMember.mpSpec = pSpec;
	gInit_ProcEndHandlerMember.mState = INIT_STT_IDLE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_Destructor
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitProcEndHandler_Destructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_GetStatus
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
EInitInitializeState CInitProcEndHandler_GetState()
{
	return gInit_ProcEndHandlerMember.mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_Start
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CInitProcEndHandler_Start()
{
	if ( gInit_ProcEndHandlerMember.mState == INIT_STT_IDLE ){
		gInit_ProcEndHandlerMember.mState = INIT_STT_RUNNING;
		CSrvcMsgActivity_Start(gInit_ProcEndHandlerMember.mpSpec->pActivity);

		return true;
	}
	else{
		// No Action
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_Stop
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CInitProcEndHandler_Stop()
{
	if ( gInit_ProcEndHandlerMember.mState == INIT_STT_RUNNING ){
		CSrvcMsgActivity_Terminate(gInit_ProcEndHandlerMember.mpSpec->pActivity);
		gInit_ProcEndHandlerMember.mState = INIT_STT_IDLE;
	}
	else{
		// No Action
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitProcEndHandler_DeviceActionResponse
// Description   : 
// Parameter     : 
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitProcEndHandler_DeviceActionResponse(ESysDeviceAction action, UC moduleID)
{
	if ( gInit_ProcEndHandlerMember.mState == INIT_STT_RUNNING ){
		CSrvcMsgActivity_SetTrigger(gInit_ProcEndHandlerMember.mpSpec->pActivity, action, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(gInit_ProcEndHandlerMember.mpSpec->pActivity) == true ){									// ƒV[ƒPƒ“ƒXŠ®—¹
			gInit_ProcEndHandlerMember.mState = INIT_STT_IDLE;
		}
		else{
			CSrvcMsgActivity_Progress(gInit_ProcEndHandlerMember.mpSpec->pActivity);
		}
	}
	else{
		// No Action
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
