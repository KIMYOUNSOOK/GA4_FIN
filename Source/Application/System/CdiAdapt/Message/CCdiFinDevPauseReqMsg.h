// File Name:	CCdiFinDevPauseReqMsg.h
// Description:	Header File of Device Pause Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevPauseReqMsg_h)
#define CCdiFinDevPauseReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinPauseRequest.h"
#include "ECdiFinRequestStatus.h"

#include "SCdiFinDevPauseReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Pause Request Message ---
typedef struct /*CCdiFinDevPauseReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevPauseReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevPauseReqMsg_Constructor(CCdiFinDevPauseReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevPauseReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevPauseReq)		// UC (CCdiFinDevPauseReqMsg*)
#define CCdiFinDevPauseReqMsg_GetSize(apThis) ((US)CDIFIN_DEV_PAUSE_REQ_MSG_SIZE)
																				// US (CCdiFinDevPauseReqMsg*)
#define CCdiFinDevPauseReqMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinDevPauseReqMsg*)
void CCdiFinDevPauseReqMsg_SetMessage(CCdiFinDevPauseReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevPauseReq;

	((struct SCdiFinDevPauseReqFrmt*)(pThis->mpMsg))->sheetID = CDIFIN_RESERVED;
}

#define CCdiFinDevPauseReqMsg_GetRequestStatus(apThis) ((UC)((struct SCdiFinDevPauseReqFrmt*)((apThis)->mpMsg))->reqStts)
#define CCdiFinDevPauseReqMsg_SetRequestStatus(apThis, aReqStts) (((struct SCdiFinDevPauseReqFrmt*)((apThis)->mpMsg))->reqStts = aReqStts)
#define CCdiFinDevPauseReqMsg_GetPauseRequest(apThis) ((UC)((struct SCdiFinDevPauseReqFrmt*)((apThis)->mpMsg))->pauseReq)
																				// UC (CCdiFinDevPauseReqMsg*)
#define CCdiFinDevPauseReqMsg_SetPauseRequest(apThis, aPauseReq) (((struct SCdiFinDevPauseReqFrmt*)((apThis)->mpMsg))->pauseReq = aPauseReq)
																				// void (CCdiFinDevPauseReqMsg*, UC)


#endif // !defined(CCdiFinDevPauseReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
