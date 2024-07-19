// File Name:	CCdiFinDevSttsReqMsg.h
// Description:	Header File of Device Status Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevSttsReqMsg_h)
#define CCdiFinDevSttsReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinSttsID.h"

#include "SCdiFinDevSttsReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Status Request Message ---
typedef struct /*CCdiFinDevSttsReqMsg*/
{
	UC*	mpMsg;
} CCdiFinDevSttsReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevSttsReqMsg_Constructor(CCdiFinDevSttsReqMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinDevSttsReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevSttsReq)		// UC (CCdiFinDevSttsReqMsg*)
#define CCdiFinDevSttsReqMsg_GetSize(apThis) ((US)CDIFIN_DEV_STTS_REQ_MSG_SIZE)	// US (CCdiFinDevSttsReqMsg*)
#define CCdiFinDevSttsReqMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinDevSttsReqMsg*)
void CCdiFinDevSttsReqMsg_SetMessage(CCdiFinDevSttsReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_DevSttsReq;
}

#define CCdiFinDevSttsReqMsg_GetStatusID(apThis) (CF2_US(&(((struct SCdiFinDevSttsReqFrmt*)((apThis)->mpMsg))->statusID_Octet0)))
																				// US (CCdiFinDevSttsReqMsg*)
#define CCdiFinDevSttsReqMsg_SetStatusID(apThis, aStatusID) (US_CF2(aStatusID, &(((struct SCdiFinDevSttsReqFrmt*)((apThis)->mpMsg))->statusID_Octet0)))
																				// void (CCdiFinDevSttsReqMsg*, US)

#endif // !defined(CCdiFinDevSttsReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
