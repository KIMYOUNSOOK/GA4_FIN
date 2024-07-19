// File Name:	CCdiFinDevCfgUpdateMsg.h
// Description:	Header File of Device Configuration Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevCfgUpdateMsg_h)
#define CCdiFinDevCfgUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevCfgUpdtFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Configuration Update Message ---
typedef struct /*CCdiFinDevCfgUpdateMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevCfgUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevCfgUpdateMsg_Constructor(CCdiFinDevCfgUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevCfgUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevCfgUpdate)	// UC (CCdiFinDevCfgUpdateMsg*)
#define CCdiFinDevCfgUpdateMsg_GetSize(apThis) ((apThis)->mSize)				// US (CCdiFinDevCfgUpdateMsg*)
#define CCdiFinDevCfgUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinDevCfgUpdateMsg*)
void CCdiFinDevCfgUpdateMsg_SetMessage(CCdiFinDevCfgUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevCfgUpdate;
}

#define CCdiFinDevCfgUpdateMsg_GetSubsystem(apThis) ((UC)((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->subsystem)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCfgUpdateMsg_SetSubsystem(apThis, aSubsystem) (((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->subsystem = aSubsystem)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCfgUpdateMsg_GetInstance(apThis) ((UC)((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->instance)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCfgUpdateMsg_SetInstance(apThis, aInstance) (((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->instance = aInstance)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCfgUpdateMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCfgUpdateMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCfgUpdateMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinDevCfgUpdtFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCfgUpdateMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_DEVCFGUPDTMINSIZE))
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCfgUpdateMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_DEVCFGUPDTMINSIZE + aSize)
																				// void (CCdiFinChgShtFinReqMsg*, UC)



#endif // !defined(CCdiFinDevCfgUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
