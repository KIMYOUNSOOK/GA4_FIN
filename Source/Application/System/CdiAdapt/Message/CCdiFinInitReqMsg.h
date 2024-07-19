// File Name:	CCdiFinInitReqMsg.h
// Description:	Header File of Initialization Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinInitReqMsg_h)
#define CCdiFinInitReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinInitReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Initialization Request Message ---
static struct SCdiFinInitReqFrmt const cCdiFin_InitReqMsg = {
	eCdiFinMsgID_InitReq
};


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Initialization Request Message ---
typedef struct /*CCdiFinInitReqMsg*/
{
	UC*	mpMsg;
} CCdiFinInitReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinInitReqMsg_Constructor(CCdiFinInitReqMsg* pThis)
{
//	pThis->mpMsg = &cCdiFin_InitReqMsg;											// 未使用のため処理時間削減のためコメントアウト
}

#define CCdiFinInitReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_InitReq)				// UC (CCdiFinInitReqMsg*)
#define CCdiFinInitReqMsg_GetSize(apThis) ((US)CDIFIN_INIT_REQ_MSG_SIZE)		// US (CCdiFinInitReqMsg*)
#define CCdiFinInitReqMsg_GetMessage(apThis) ((UC *)&cCdiFin_InitReqMsg)		// UC* (CCdiFinInitReqMsg*)
void CCdiFinInitReqMsg_SetMessage(CCdiFinInitReqMsg* pThis, UC* pMsg, US size)
{
	// No Action
}


#endif // !defined(CCdiFinInitReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
