// File Name:	CCdiFinJamSttsCheckMsg.h
// Description:	Header File of Jam Status Check Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinJamSttsCheckMsg_h)
#define CCdiFinJamSttsCheckMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinCheckTrigger.h"

#include "SCdiFinJamSttsCheckFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Jam Status Check Message ---
typedef struct /*CCdiFinJamSttsCheckMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinJamSttsCheckMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinJamSttsCheckMsg_Constructor(CCdiFinJamSttsCheckMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinJamSttsCheckMsg_GetID(apThis) ((UC)eCdiFinMsgID_ReqJamSttsCheck)	// UC (CCdiFinJamSttsCheckMsg*)
#define CCdiFinJamSttsCheckMsg_GetSize(apThis) ((US)CDIFIN_JAM_STTS_CHECK_MSG_SIZE)
																				// US (CCdiFinJamSttsCheckMsg*)
#define CCdiFinJamSttsCheckMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinJamSttsCheckMsg*)
void CCdiFinJamSttsCheckMsg_SetMessage(CCdiFinJamSttsCheckMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_ReqJamSttsCheck;
}

#define CCdiFinJamSttsCheckMsg_GetCheckTrigger(apThis) ((UC)((struct SCdiFinJamSttsCheckFrmt*)((apThis)->mpMsg))->trigger)
																				// UC (CCdiFinJamSttsCheckMsg*)
#define CCdiFinJamSttsCheckMsg_SetCheckTrigger(apThis, aTrigger) (((struct SCdiFinJamSttsCheckFrmt*)((apThis)->mpMsg))->trigger = aTrigger)
																				// void (CCdiFinJamSttsCheckMsg*, UC)


#endif // !defined(CCdiFinJamSttsCheckMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
