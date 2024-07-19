// File Name:	CInitShtInitHandler.c
// Description:	Implimentation File of Sheet Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "CInitShtInitHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void StartInitialize(CInitShtInitHandler* pThis);
void StopInitialize(CInitShtInitHandler* pThis);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_Constructor
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitShtInitHandler_Constructor(CInitShtInitHandler* pThis, const InitShtInitHandlerSpec* pSpec)
{
	pThis->mpSpec	= pSpec;
	pThis->mState	= INIT_STT_IDLE;
	pThis->mSheetNo	= SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_Destructor
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitShtInitHandler_Destructor(CInitShtInitHandler* pThis)
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : StartInitialize
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void StartInitialize(CInitShtInitHandler* pThis)
{
	CSrvcMsgActivity_Start(pThis->mpSpec->pActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : StopInitialize
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void StopInitialize(CInitShtInitHandler* pThis)
{
	CSrvcMsgActivity_Terminate(pThis->mpSpec->pActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_GetStatus
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
EInitInitializeState CInitShtInitHandler_GetState(CInitShtInitHandler* pThis)
{
	return (EInitInitializeState)pThis->mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_Start
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CInitShtInitHandler_Start(CInitShtInitHandler* pThis, UC sheetNo)
{
	if ( pThis->mState == INIT_STT_IDLE ){
		pThis->mState	= INIT_STT_RUNNING;
		pThis->mSheetNo	= sheetNo;
		StartInitialize(pThis);

		return true;
	}
	else{
		// No Action
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_Stop
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CInitShtInitHandler_Stop(CInitShtInitHandler* pThis, UC sheetNo)
{
	if ( (pThis->mState == INIT_STT_RUNNING) &&
		 ( (sheetNo == pThis->mSheetNo) || (sheetNo == SHEETNO_NUM) ) ){
		StopInitialize(pThis);
		pThis->mSheetNo	= SHEETNO_NUM;
		pThis->mState	= INIT_STT_IDLE;
	}
	else{
		// No Action
	}

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitShtInitHandler_SheetActionResponse
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CInitShtInitHandler_SheetActionResponse(CInitShtInitHandler* pThis, UC sheetNo, EShtSheetAction action, UC moduleID)
{
	if ( (pThis->mSheetNo == sheetNo) &&
		 (pThis->mState == INIT_STT_RUNNING) ){
		 CSrvcMsgActivity_SetTrigger(pThis->mpSpec->pActivity, action, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(pThis->mpSpec->pActivity) == true ){									// ƒV[ƒPƒ“ƒXŠ®—¹
			pThis->mSheetNo = SHEETNO_NUM;
			pThis->mState	= INIT_STT_IDLE;
//@ Sheet Initialize Manager‚ÉŠ®—¹•ñ
		}
		else{
			CSrvcMsgActivity_Progress(pThis->mpSpec->pActivity);
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
