// File Name:	CCdiFinDevCfgReqMsg.h
// Description:	Header File of Device Configuration Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevCfgReqMsg_h)
#define CCdiFinDevCfgReqMsg_h

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

#include "SCdiFinDevCfgReqFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct /*CCdiFinDevCfgReqMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevCfgReqMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevCfgReqMsg_Constructor(CCdiFinDevCfgReqMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevCfgReqMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevCfgReq)			// UC (CCdiFinDevCfgReqMsg*)
#define CCdiFinDevCfgReqMsg_GetSize(apThis) ((US)CDIFIN_DEV_CFG_REQ_MSG_SIZE)	// US (CCdiFinDevCfgReqMsg*)
#define CCdiFinDevCfgReqMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinDevCfgReqMsg*)
void CCdiFinDevCfgReqMsg_SetMessage(CCdiFinDevCfgReqMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevCfgReq;
}

#define CCdiFinDevCfgReqMsg_GetSubsystem(apThis) ((UC)((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->subsystem)
																				// UC (CCdiFinDevCfgReqMsg*)
#define CCdiFinDevCfgReqMsg_SetSubsystem(apThis, aSubsystem) (((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->subsystem = aSubsystem)
																				// void (CCdiFinDevCfgReqMsg*, UC)
#define CCdiFinDevCfgReqMsg_GetInstance(apThis) ((UC)((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->instance)
																				// UC (CCdiFinDevCfgReqMsg*)
#define CCdiFinDevCfgReqMsg_SetInstance(apThis, aInstance) (((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->instance = aInstance)
																				// void (CCdiFinDevCfgReqMsg*, UC)
#define CCdiFinDevCfgReqMsg_GetAttrKey(apThis) ((UC)((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->attrKey)
																				// UC (CCdiFinDevCfgReqMsg*)
#define CCdiFinDevCfgReqMsg_SetAttrKey(apThis, aAttrKey) (((struct SCdiFinDevCfgReqFrmt*)((apThis)->mpMsg))->attrKey = aAttrKey)
																				// void (CCdiFinDevCfgReqMsg*, UC)


#endif // !defined(CCdiFinDevCfgReqMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
