// File Name:	CSeqProcEndHandler.h
// Description:	Header File of Process End Action Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSeqProcEndHandler_h
#define CSeqProcEndHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESeqActionState.h"

#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Process End Action Handler ---
typedef struct /*SeqProcEndHandlerSpec*/
{
	const SrvcMsgActivity* pActivity;						// アクティビティクラスインスタンスポインタ
	UC msgActivityNum;
} SeqProcEndHandlerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSeqProcEndHandler_Constructor();

ESeqActionState CSeqProcEndHandler_GetState();
ESeqActionState CSeqProcEndHandler_Start();
ESeqActionState CSeqProcEndHandler_DeviceActionResponse(US eventID, UC moduleID);


#endif // #ifndef CSeqProcEndHandler_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
