// File Name:	CCdiFinDevCapaReqMsg.h
// Description:	Header File of Device Capabilities Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevCapaReqMsg_h)
#define CCdiFinDevCapaReqMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinSubsys.h"
#include "ECdiFinDevCont.h"
#include "ECdiFinOutDest.h"
#include "ECdiFinInSrc.h"
#include "ECdiFinCapaAttrKeyDevCont.h"
#include "ECdiFinCapaAttrKeyOutDest.h"
#include "ECdiFinCapaAttrKeyInSrc.h"

#include "SCdiFinDevCapaReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Capabilities Request Message ---
typedef struct /*CCdiFinDevCapaReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevCapaReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevCapaReqMsg_Constructor(CCdiFinDevCapaReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevCapaReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevCapabilitiesReq)// UC (CCdiFinDevCapaReqMsg*)
#define CCdiFinDevCapaReqMsg_GetSize(apThis) ((US)CDIFIN_DEV_CAPA_REQ_MSG_SIZE)	// US (CCdiFinDevCapaReqMsg*)
#define CCdiFinDevCapaReqMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))		// UC* (CCdiFinDevCapaReqMsg*)
void CCdiFinDevCapaReqMsg_SetMessage(CCdiFinDevCapaReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevCapabilitiesReq;
}

#define CCdiFinDevCapaReqMsg_GetSubsystem(apThis) ((UC)((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->subsystem)
																				// UC (CCdiFinDevCapaReqMsg*)
#define CCdiFinDevCapaReqMsg_SetSubsystem(apThis, aSubsystem) (((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->subsystem = aSubsystem)
																				// void (CCdiFinDevCapaReqMsg*, UC)
#define CCdiFinDevCapaReqMsg_GetInstance(apThis) ((UC)((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->instance)
																				// UC (CCdiFinDevCapaReqMsg*)
#define CCdiFinDevCapaReqMsg_SetInstance(apThis, aInstance) (((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->instance = aInstance)
																				// void (CCdiFinDevCapaReqMsg*, UC)
#define CCdiFinDevCapaReqMsg_GetAttrKey(apThis) ((UC)((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->attrKey)
																				// UC (CCdiFinDevCapaReqMsg*)
#define CCdiFinDevCapaReqMsg_SetAttrKey(apThis, aAttrKey) (((struct SCdiFinDevCapaReqFrmt*)((apThis)->mpMsg))->attrKey = aAttrKey)
																				// void (CCdiFinDevCapaReqMsg*, UC)


#endif // !defined(CCdiFinDevCapaReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
