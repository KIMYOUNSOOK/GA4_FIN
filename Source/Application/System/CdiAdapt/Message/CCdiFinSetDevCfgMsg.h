// File Name:	CCdiFinSetDevCfgMsg.h
// Description:	Header File of Set Device Configuration Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSetDevCfgMsg_h)
#define CCdiFinSetDevCfgMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinSubsys.h"
#include "ECdiFinDevCont.h"
#include "ECdiFinInSrc.h"
#include "ECdiFinOutDest.h"
#include "ECdiFinCfgAttrKeyDevCont.h"
#include "ECdiFinCfgAttrKeyOutDest.h"
#include "ECdiFinCfgAttrKeyInSrc.h"

#include "SCdiFinSetDevCfgFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Set Device Configuration Message ---
typedef struct /*CCdiFinSetDevCfgMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinSetDevCfgMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinSetDevCfgMsg_Constructor(CCdiFinSetDevCfgMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = CDIFIN_SET_DEV_CFG_MIN_SIZE;
}

#define CCdiFinSetDevCfgMsg_GetID(apThis) ((UC)eCdiFinMsgID_SetDevCfg)			// UC (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_GetSize(apThis) (apThis->mSize)						// US (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinSetDevCfgMsg*)
void CCdiFinSetDevCfgMsg_SetMessage(CCdiFinSetDevCfgMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SetDevCfg;
}

#define CCdiFinSetDevCfgMsg_GetSubsystem(apThis) ((UC)((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->subsystem)
																				// UC (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_SetSubsystem(apThis, aSubsystem) (((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->subsystem = aSubsystem)
																				// void (CCdiFinSetDevCfgMsg*, UC)
#define CCdiFinSetDevCfgMsg_GetInstance(apThis) ((UC)((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->instance)
																				// UC (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_SetInstance(apThis, aInstance) (((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->instance = aInstance)
																				// void (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_GetNumAttr(apThis) ((UC)((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->numAttr)
																				// UC (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_SetNumAttr(apThis, aNumAttr) (((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->numAttr = aNumAttr)
																				// void (CCdiFinSetDevCfgMsg*, UC)
#define CCdiFinSetDevCfgMsg_GetAttrPairs(apThis) ((UC*)((struct SCdiFinSetDevCfgFrmt*)((apThis)->mpMsg))->attrPair)
																				// UC* (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_GetAttrPairsSize(apThis) ((UC)((apThis)->mSize - CDIFIN_SET_DEV_CFG_MIN_SIZE))
																				// UC (CCdiFinSetDevCfgMsg*)
#define CCdiFinSetDevCfgMsg_SetAttrPairsSize(apThis, aSize) ((apThis)->mSize = CDIFIN_SET_DEV_CFG_MIN_SIZE + aSize)
																				// void (CCdiFinSlctFinAttrMsg*, UC)

#endif // !defined(_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
