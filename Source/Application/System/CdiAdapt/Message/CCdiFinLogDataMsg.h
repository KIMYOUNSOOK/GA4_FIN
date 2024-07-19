// File Name:	CCdiFinLogDataMsg.h
// Description:	Header File of Device Controller Information Notify Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinLogDataMsg_h)
#define CCdiFinLogDataMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinLogDataInfoFrmt.h"

#include "CCdiFinLogDataInfoCommInCdiFin.h"


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
} CCdiFinLogDataInfoMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinLogDataInfoMsg_Constructor(CCdiFinLogDataInfoMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinLogDataInfoMsg_GetID(apThis) ((UC)eCdiFinMsgID_LogData)// UC (CCdiFinDevContInfoMsg*)
#define CCdiFinLogDataInfoMsg_GetSize(apThis) ((apThis)->mSize)					// US (CCdiFinDevContInfoMsg*)
#define CCdiFinLogDataInfoMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinDevContInfoMsg*)
void CCdiFinLogDataInfoMsg_SetMessage(CCdiFinLogDataInfoMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_LogData;
}
#define CCdiFinLogDataInfoMsg_GetInformationID(apThis) ((UC)((SCdiFinLogDataInfoFrmt*)((apThis)->mpMsg))->infoID)
																				// UC (CCdiFinLogDataInfoMsg*)
#define CCdiFinLogDataInfoMsg_SetInformationID(apThis, aInfoID) (((struct SCdiFinLogDataInfoFrmt*)((apThis)->mpMsg))->infoID = aInfoID)
																				// void (CCdiFinLogDataInfoMsg*, UC)
#define CCdiFinLogDataInfoMsg_GetInfoData(apThis) ((UC*)&(((SCdiFinLogDataInfoFrmt*)((apThis)->mpMsg))->infoData[0]))
																				// UC* (CCdiFinLogDataInfoMsg*)
#define CCdiFinLogDataInfoMsg_SetInfoDataSize(apThis, aSize) ((apThis)->mSize = CDIFIN_DEVCONTINFOHEADSIZE + aSize)
																				// void (CCdiFinLogDataInfoMsg*, UC)


#endif // !defined(CCdiFinLogDataInfoMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

