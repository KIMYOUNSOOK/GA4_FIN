// File Name:	CCdiFinAbnormalFinishingMsg.h
// Description:	Header File of Abnormal Finishing Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinAbnormalFinishingMsg_h)
#define CCdiFinAbnormalFinishingMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinAbnormalFinishingFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Abnormal Finishing Message ---
typedef struct /*CCdiFinAbnormalFinishingMsg*/
{
	UC*	mpMsg;
} CCdiFinAbnormalFinishingMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinAbnormalFinishingMsg_Constructor(CCdiFinAbnormalFinishingMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinAbnormalFinishingMsg_GetID(apThis) ((UC)eCdiFinMsgID_AbnormalFinishing)
																				// UC (CCdiFinAbnormalFinishingMsg*)
#define CCdiFinAbnormalFinishingMsg_GetSize(apThis) ((US)CDIFIN_ABNORMAL_FINISHING_MSG_SIZE)
																				// US (CCdiFinAbnormalFinishingMsg*)
#define CCdiFinAbnormalFinishingMsg_GetMessage(apThis) ((apThis)->mpMsg)		// UC* (CCdiFinAbnormalFinishingMsg*)
void CCdiFinAbnormalFinishingMsg_SetMessage(CCdiFinAbnormalFinishingMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_AbnormalFinishing;
}

#define CCdiFinAbnormalFinishingMsg_GetPurgePossibility(apThis) ((UC)(((struct SCdiFinAbnormalFinishingFrmt*)((apThis)->mpMsg))->purgePossibility))
																				// UC (CCdiFinAbnormalFinishingMsg*)
#define CCdiFinAbnormalFinishingMsg_SetPurgePossibility(apThis, aPossibility) (((struct SCdiFinAbnormalFinishingFrmt*)((apThis)->mpMsg))->purgePossibility = aPossibility)
																				// void (CCdiFinAbnormalFinishingMsg*, UC)
#define CCdiFinAbnormalFinishingMsg_GetAbortSheetID(apThis) ((UC)(((struct SCdiFinAbnormalFinishingFrmt*)((apThis)->mpMsg))->abortSheetID))
																				// UC (CCdiFinAbnormalFinishingMsg*)
#define CCdiFinAbnormalFinishingMsg_SetAbortSheetID(apThis, aSheetID) (((struct SCdiFinAbnormalFinishingFrmt*)((apThis)->mpMsg))->abortSheetID = aSheetID)
																				// void (CCdiFinAbnormalFinishingMsg*, UC)


#endif // !defined(CCdiFinAbnormalFinishingMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
