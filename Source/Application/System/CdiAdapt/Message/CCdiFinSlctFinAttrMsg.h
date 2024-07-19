// File Name:	CCdiFinSlctFinAttrMsg.h
// Description:	Header File of Select Finisher Attribute Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSlctFinAttrMsg_h)
#define CCdiFinSlctFinAttrMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinOutDest.h"

#include "SCdiFinSlctFinAttrFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Select Finisher Attribute Message ---
typedef struct /*CCdiFinSlctFinAttrMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinSlctFinAttrMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinSlctFinAttrMsg_Constructor(CCdiFinSlctFinAttrMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinSlctFinAttrMsg_GetID(apThis) ((UC)eCdiFinMsgID_SelectFinAttr)	// UC (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_GetSize(apThis) ((apThis)->mSize)					// US (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinSlctFinAttrMsg*)
void CCdiFinSlctFinAttrMsg_SetMessage(CCdiFinSlctFinAttrMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SelectFinAttr;
}

#define CCdiFinSlctFinAttrMsg_GetDestID(apThis) ((UC)((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->destID)
																				// UC (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_SetDestID(apThis, aDestID) (((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->destID = aDestID)
																				// void (CCdiFinSlctFinAttrMsg*, UC)
#define CCdiFinSlctFinAttrMsg_GetBinSelect(apThis) ((UC)((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->binSlct)
																				// UC (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_SetBinSelect(apThis, aBinSlct) (((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->binSlct = aBinSlct)
																				// void (CCdiFinSlctFinAttrMsg*, UC)
#define CCdiFinSlctFinAttrMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinSlctFinAttrMsg*, UC)
#define CCdiFinSlctFinAttrMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinSlctFinAttrFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_SLCT_FIN_ATTR_MIN_SIZE))
																				// UC (CCdiFinSlctFinAttrMsg*)
#define CCdiFinSlctFinAttrMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_SLCT_FIN_ATTR_MIN_SIZE + aSize)
																				// void (CCdiFinSlctFinAttrMsg*, UC)


#endif // !defined(CCdiFinSlctFinAttrMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
