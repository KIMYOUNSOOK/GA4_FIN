// File Name:	CCdiFinDevContInfoMsg.h
// Description:	Header File of Device Controller Information Notify Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContInfoMsg_h)
#define CCdiFinDevContInfoMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevContInfoFrmt.h"

#include "CCdiFinDevContInfoCancelStaple.h"
#include "CCdiFinDevContInfoCommInCdiFin.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Controller Information Notify Message ---
typedef struct /*CCdiFinDevContInfoMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDevContInfoMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinDevContInfoMsg_Constructor(CCdiFinDevContInfoMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDevContInfoMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevContInfoNotify)// UC (CCdiFinDevContInfoMsg*)
#define CCdiFinDevContInfoMsg_GetSize(apThis) ((apThis)->mSize)					// US (CCdiFinDevContInfoMsg*)
#define CCdiFinDevContInfoMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinDevContInfoMsg*)
void CCdiFinDevContInfoMsg_SetMessage(CCdiFinDevContInfoMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DevContInfoNotify;
}
#define CCdiFinDevContInfoMsg_GetInformationID(apThis) ((UC)((struct SCdiFinDevContInfoFrmt*)((apThis)->mpMsg))->infoID)
																				// UC (CCdiFinDevContInfoMsg*)
#define CCdiFinDevContInfoMsg_SetInformationID(apThis, aInfoID) (((struct SCdiFinDevContInfoFrmt*)((apThis)->mpMsg))->infoID = aInfoID)
																				// void (CCdiFinDevContInfoMsg*, UC)
#define CCdiFinDevContInfoMsg_GetInfoData(apThis) ((UC*)&(((struct SCdiFinDevContInfoFrmt*)((apThis)->mpMsg))->infoData[0]))
																				// UC* (CCdiFinDevContInfoMsg*)
#define CCdiFinDevContInfoMsg_SetInfoDataSize(apThis, aSize) ((apThis)->mSize = CDIFIN_DEVCONTINFOHEADSIZE + aSize)
																				// void (CCdiFinDevContInfoMsg*, UC)


#endif // !defined(CCdiFinDevContInfoMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ Device Controller Information Notify / Cancel Stapleメッセージ送信対応 ]
