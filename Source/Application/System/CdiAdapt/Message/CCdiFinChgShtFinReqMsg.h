// File Name:	CCdiFinChgShtFinReqMsg.h
// Description:	Header File of Change Sheet Finish Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinChgShtFinReqMsg_h)
#define CCdiFinChgShtFinReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinChgShtFinReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Change Sheet Finish Request Message ---
typedef struct /*CCdiFinChgShtFinReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinChgShtFinReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinChgShtFinReqMsg_Constructor(CCdiFinChgShtFinReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinChgShtFinReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_ChgShtFinReq)	// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_GetSize(apThis) ((apThis)->mSize)				// US (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinChgShtFinReqMsg*)
void CCdiFinChgShtFinReqMsg_SetMessage(CCdiFinChgShtFinReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_ChgShtFinReq;
}

#define CCdiFinChgShtFinReqMsg_GetSetID(apThis) ((UC)((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->setID)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_SetSetID(apThis, aSetID) (((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinChgShtFinReqMsg_GetSheetID(apThis) ((UC)((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->sheetID)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinChgShtFinReqMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinChgShtFinReqMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinChgShtFinReqFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_CHG_SHT_REQ_MIN_SIZE))
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinChgShtFinReqMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_CHG_SHT_REQ_MIN_SIZE + aSize)
																				// void (CCdiFinChgShtFinReqMsg*, UC)

#endif // !defined(CCdiFinChgShtFinReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
