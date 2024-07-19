// File Name:	CCdiFinSetFinReqMsg.h
// Description:	Header File of Set Finish Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSetFinReqMsg_h)
#define CCdiFinSetFinReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinSetFinReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Set Finish Request Message ---
typedef struct /*CCdiFinSetFinReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinSetFinReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinSetFinReqMsg_Constructor(CCdiFinSetFinReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinSetFinReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_SetFinReq)			// UC (CCdiFinSetFinReqMsg*)
#define CCdiFinSetFinReqMsg_GetSize(apThis) ((US)CDIFIN_SET_FIN_REQ_MSG_SIZE)	// US (CCdiFinSetFinReqMsg*)
#define CCdiFinSetFinReqMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinSetFinReqMsg*)
void CCdiFinSetFinReqMsg_SetMessage(CCdiFinSetFinReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SetFinReq;

	((struct SCdiFinSetFinReqFrmt*)(pThis->mpMsg))->destID	= CDIFIN_RESERVED;
	((struct SCdiFinSetFinReqFrmt*)(pThis->mpMsg))->binSlct	= CDIFIN_RESERVED;
	((struct SCdiFinSetFinReqFrmt*)(pThis->mpMsg))->setID	= CDIFIN_RESERVED;
}

#define CCdiFinSetFinReqMsg_GetSetEndAct(apThis) ((UC)((struct SCdiFinSetFinReqFrmt*)((apThis)->mpMsg))->setEndAct)
																				// UC (CCdiFinSetFinReqMsg*)
#define CCdiFinSetFinReqMsg_SetSetEndAct(apThis, aEndAct) (((struct SCdiFinSetFinReqFrmt*)((apThis)->mpMsg))->setEndAct = aEndAct)
																				// void (CCdiFinSetFinReqMsg*, UC)


#endif // !defined(CCdiFinSetFinReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
