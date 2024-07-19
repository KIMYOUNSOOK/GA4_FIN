// File Name:	CInitProcEndHandler.h
// Description:	Header File of Process End Action Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CInitProcEndHandler_h)
#define CInitProcEndHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "EInitInitializeState.h"
#include "ESysDeviceAction.h"
#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Process End Action Handler ---
typedef struct /*InitProcEndHandlerSpec*/
{
	CSrvcMsgActivity* pActivity;							// アクティビティクラスインスタンスポインタ
} InitProcEndHandlerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CInitProcEndHandler_Constructor(const InitProcEndHandlerSpec* pSpec);
void CInitProcEndHandler_Destructor();

EInitInitializeState CInitProcEndHandler_GetState();
bool CInitProcEndHandler_Start();
bool CInitProcEndHandler_Stop();
void CInitProcEndHandler_DeviceActionResponse(ESysDeviceAction action, UC moduleID);


#endif // !defined(CInitProcEndHandler_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
