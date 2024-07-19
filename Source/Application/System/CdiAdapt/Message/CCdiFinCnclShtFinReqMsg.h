// File Name:	CCdiFinCnclShtFinReqMsg.h
// Description:	Header File of Cancel Sheet Finish Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinCnclShtFinReqMsg_h)
#define CCdiFinCnclShtFinReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinCnclShtFinReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Cancel Sheet Finish Request Message ---
typedef struct /*CCdiFinCnclShtFinReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinCnclShtFinReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinCnclShtFinReqMsg_Constructor(CCdiFinCnclShtFinReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinCnclShtFinReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_CancelSheetFinReq)
																				// UC (CCdiFinCnclShtFinReqMsg*)
#define CCdiFinCnclShtFinReqMsg_GetSize(apThis) ((US)CDIFIN_CANCEL_SHT_FIN_REQ_MSG_SIZE)
																				// US (CCdiFinCnclShtFinReqMsg*)
#define CCdiFinCnclShtFinReqMsg_GetMessage(apThis) ((apThis)->mpMsg)			// UC* (CCdiFinCnclShtFinReqMsg*)
void CCdiFinCnclShtFinReqMsg_SetMessage(CCdiFinCnclShtFinReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_CancelSheetFinReq;
}

#define CCdiFinCnclShtFinReqMsg_GetSetID(apThis) ((UC)((struct SCdiFinCnclShtFinReqFrmt*)((apThis)->mpMsg))->setID)
																				// UC (CCdiFinCnclShtFinReqMsg*)
#define CCdiFinCnclShtFinReqMsg_SetSetID(apThis, aSetID) (((struct SCdiFinCnclShtFinReqFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinCnclShtFinReqMsg*, UC)
#define CCdiFinCnclShtFinReqMsg_GetSheetID(apThis) ((UC)((struct SCdiFinCnclShtFinReqFrmt*)((apThis)->mpMsg))->sheetID)
																				// UC (CCdiFinCnclShtFinReqMsg*)
#define CCdiFinCnclShtFinReqMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinCnclShtFinReqFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinCnclShtFinReqMsg*, UC)


#endif // !defined(CCdiFinCnclShtFinReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
