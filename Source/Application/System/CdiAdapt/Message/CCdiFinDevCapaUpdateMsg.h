// File Name:	CCdiFinDevCapaUpdateMsg.h
// Description:	Header File of Device Capabilities Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevCapaUpdateMsg_h)
#define CCdiFinDevCapaUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevCapaUpdtFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Capabilities Update Message ---
typedef struct /*CCdiFinDevCapaUpdateMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevCapaUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevCapaUpdateMsg_Constructor(CCdiFinDevCapaUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevCapaUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevCapabilitiesUpdate)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_GetSize(apThis) ((apThis)->mSize)				// US (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)			// UC* (CCdiFinChgShtFinReqMsg*)
void CCdiFinDevCapaUpdateMsg_SetMessage(CCdiFinDevCapaUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevCapabilitiesUpdate;
}

#define CCdiFinDevCapaUpdateMsg_GetSubsystem(apThis) ((UC)((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->subsystem)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_SetSubsystem(apThis, aSubsystem) (((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->subsystem = aSubsystem)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCapaUpdateMsg_GetInstance(apThis) ((UC)((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->instance)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_SetInstance(apThis, aInstance) (((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->instance = aInstance)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCapaUpdateMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinChgShtFinReqMsg*, UC)
#define CCdiFinDevCapaUpdateMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinDevCapaUpdtFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_DEV_CAPA_UPDT_MIN_SIZE))
																				// UC (CCdiFinChgShtFinReqMsg*)
#define CCdiFinDevCapaUpdateMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_DEV_CAPA_UPDT_MIN_SIZE + aSize)
																				// void (CCdiFinChgShtFinReqMsg*, UC)


#endif // !defined(CCdiFinDevCapaUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
