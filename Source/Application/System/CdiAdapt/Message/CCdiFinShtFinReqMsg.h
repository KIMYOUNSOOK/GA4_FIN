// File Name:	CCdiFinShtFinReqMsg.h
// Description:	Header File of Sheet Finish Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtFinReqMsg_h)
#define CCdiFinShtFinReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinShtFinReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Finish Request Message ---
typedef struct /*CCdiFinShtFinReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinShtFinReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinShtFinReqMsg_Constructor(CCdiFinShtFinReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinShtFinReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_SheetFinReq)		// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_GetSize(apThis) ((apThis)->mSize)					// US (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinShtFinReqMsg*)
void CCdiFinShtFinReqMsg_SetMessage(CCdiFinShtFinReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SheetFinReq;
}

#define CCdiFinShtFinReqMsg_GetDestID(apThis) ((UC)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->destID)
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetDestID(apThis, aDestID) (((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->destID = aDestID)
																				// void (CCdiFinShtFinReqMsg*, UC)
#define CCdiFinShtFinReqMsg_GetBinSelect(apThis) ((UC)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->binSlct)
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetBinSelect(apThis, aBinSlct) (((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->binSlct = aBinSlct)
																				// void (CCdiFinShtFinReqMsg*, UC)
#define CCdiFinShtFinReqMsg_GetSetID(apThis) ((UC)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->setID)
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetSetID(apThis, aSetID) (((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinShtFinReqMsg*, UC)
#define CCdiFinShtFinReqMsg_GetSheetID(apThis) ((UC)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->sheetID)
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinShtFinReqMsg*, UC)
#define CCdiFinShtFinReqMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinShtFinReqMsg*, UC)
#define CCdiFinShtFinReqMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinShtFinReqFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_SHT_FIN_REQ_MIN_SIZE))
																				// UC (CCdiFinShtFinReqMsg*)
#define CCdiFinShtFinReqMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_SHT_FIN_REQ_MIN_SIZE + aSize)
																				// void (CCdiFinShtFinReqMsg*, UC)


#endif // !defined(CCdiFinShtFinReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
