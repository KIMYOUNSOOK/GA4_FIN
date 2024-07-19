// File Name:	CInitShtInitHandler.h
// Description:	Header File of Sheet Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CInitShtInitHandler_h)
#define CInitShtInitHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "EInitInitializeState.h"
#include "EShtSheetAction.h"
#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Device Initialize Handler ---
typedef struct /*InitShtInitHandlerSpec*/
{
	CSrvcMsgActivity* pActivity;							// アクティビティクラスインスタンスポインタ
} InitShtInitHandlerSpec;

// --- Sheet Initialize Handler ---
typedef struct /*CInitShtInitHandler*/
{
	const InitShtInitHandlerSpec* mpSpec;					// Pointer of Spec Data
	UC mState;												// Sheet Initialize State
	UC mSheetNo;											// Sheet No.
} CInitShtInitHandler;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CInitShtInitHandler_Constructor(CInitShtInitHandler* pThis, const InitShtInitHandlerSpec* pSpec);
void CInitShtInitHandler_Destructor(CInitShtInitHandler* pThis);

EInitInitializeState CInitShtInitHandler_GetState(CInitShtInitHandler* pThis);
bool CInitShtInitHandler_Start(CInitShtInitHandler* pThis, UC sheetNo);
bool CInitShtInitHandler_Stop(CInitShtInitHandler* pThis, UC sheetNo);
void CInitShtInitHandler_SheetActionResponse(CInitShtInitHandler* pThis, UC sheetNo, EShtSheetAction action, UC moduleID);


#endif // !defined(CInitShtInitHandler_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
