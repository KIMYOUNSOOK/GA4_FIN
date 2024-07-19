// File Name:	CCdiFinSysContInfoMsg.h
// Description:	Header File of System Controller Information Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSysContInfoMsg_h)
#define CCdiFinSysContInfoMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinSysContInfoID.h"

#include "SCdiFinSysContInfoFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- System Controller Information Message ---
typedef struct /*CCdiFinSysContInfoMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinSysContInfoMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinSysContInfoMsg_Constructor(CCdiFinSysContInfoMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = CDIFIN_SET_DEV_CFG_MIN_SIZE;
}

#define CCdiFinSysContInfoMsg_GetID(apThis) ((UC)eCdiFinMsgID_SysContInfoNotify)	// UC (CCdiFinSysContInfoMsg*)
#define CCdiFinSysContInfoMsg_GetSize(apThis) (apThis->mSize)						// US (CCdiFinSysContInfoMsg*)
#define CCdiFinSysContInfoMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinSysContInfoMsg*)
void CCdiFinSysContInfoMsg_SetMessage(CCdiFinSysContInfoMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SysContInfoNotify;
}

#define CCdiFinSysContInfoMsg_GetInformationID(apThis) ((UC)((struct SCdiFinSysContInfoFrmt*)((apThis)->mpMsg))->infoID)
																				// UC (CCdiFinSysContInfoMsg*)
#define CCdiFinSysContInfoMsg_SetInformationID(apThis, aInfoID) (((struct SCdiFinSysContInfoFrmt*)((apThis)->mpMsg))->infoID = aInfoID)
																				// void (CCdiFinSysContInfoMsg*, UC)
#define CCdiFinSysContInfoMsg_GetInfoData(apThis) ((UC*)(&((struct SCdiFinSysContInfoFrmt*)((apThis)->mpMsg))->infoData[0]))
																				// UC* (CCdiFinSysContInfoMsg*)
#define CCdiFinSysContInfoMsg_GetInfoDataSize(apThis) ((UC)((apThis)->mSize - CDIFIN_SYS_CONT_INFO_HEAD_SIZE))
																				// UC (CCdiFinSysContInfoMsg*)
#define CCdiFinSysContInfoMsg_SetInfoDataSize(apThis, aSize) ((apThis)->mSize = CDIFIN_SYS_CONT_INFO_HEAD_SIZE + aSize)
																				// void (CCdiFinSysContInfoMsg*, UC)

#endif // !defined(CCdiFinSysContInfoMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
