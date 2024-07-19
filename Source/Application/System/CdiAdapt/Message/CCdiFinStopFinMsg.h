// File Name:	CCdiFinStopFinMsg.h
// Description:	Header File of Stop Finisher Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinStopFinMsg_h)
#define CCdiFinStopFinMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinShutDwnType.h"

#include "SCdiFinStopFinFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Set Finish Request Message ---
typedef struct /*CCdiFinStopFinMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinStopFinMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinStopFinMsg_Constructor(CCdiFinStopFinMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinStopFinMsg_GetID(apThis) ((UC)eCdiFinMsgID_StopFinisher)			// UC (CCdiFinStopFinMsg*)
#define CCdiFinStopFinMsg_GetSize(apThis) ((US)CDIFIN_STOP_FIN_MSG_SIZE)
																				// US (CCdiFinStopFinMsg*)
#define CCdiFinStopFinMsg_GetMessage(apThis) ((apThis)->mpMsg)					// UC* (CCdiFinStopFinMsg*)
void CCdiFinStopFinMsg_SetMessage(CCdiFinStopFinMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_StopFinisher;
}

#define CCdiFinStopFinMsg_GetShutDownType(apThis) ((UC)((struct SCdiFinStopFinFrmt*)((apThis)->mpMsg))->shutDownType)
																				// UC (CCdiFinStopFinMsg*)
#define CCdiFinStopFinMsg_SetShutDownType(apThis, aDwnType) (((struct SCdiFinStopFinFrmt*)((apThis)->mpMsg))->shutDownType = aDwnType)
																				// void (CCdiFinStopFinMsg*, UC)


#endif // !defined(CCdiFinStopFinMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
