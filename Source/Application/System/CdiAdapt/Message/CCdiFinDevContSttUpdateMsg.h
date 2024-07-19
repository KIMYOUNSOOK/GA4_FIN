// File Name:	CCdiFinDevContSttUpdateMsg.h
// Description:	Header File of Device Controller State Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContSttUpdateMsg_h)
#define CCdiFinDevContSttUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevContSttUpdateFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Controller State Update Message ---
typedef struct /*CCdiFinDevContSttUpdateMsg*/
{
	UC*	mpMsg;
} CCdiFinDevContSttUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinDevContSttUpdateMsg_Constructor(CCdiFinDevContSttUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinDevContSttUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevContSttUpdate)
																				// UC (CCdiFinDevContSttUpdateMsg*)
#define CCdiFinDevContSttUpdateMsg_GetSize(apThis) ((US)CDIFIN_DEV_CONT_STT_UPDT_MSG_SIZE)
																				// US (CCdiFinDevContSttUpdateMsg*)
#define CCdiFinDevContSttUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)			// UC* (CCdiFinDevContSttUpdateMsg*)
void CCdiFinDevContSttUpdateMsg_SetMessage(CCdiFinDevContSttUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_DevContSttUpdate;

	// --- 未使用Bitのクリア ---
	((struct SCdiFinDevContSttUpdateFrmt *)(pThis->mpMsg))->devContState &= ( ((UC)0x03 << 0) | ((UC)0x03 << 5) );
}
#define CCdiFinDevContSttUpdateMsg_GetProductionState(apThis) ((UC)(((struct SCdiFinDevContSttUpdateFrmt*)((apThis)->mpMsg))->devContState & 0x03))
																				// UC (CCdiFinDevContSttUpdateMsg*)
void CCdiFinDevContSttUpdateMsg_SetProductionState(CCdiFinDevContSttUpdateMsg* pThis, UC prdctStt)
{
	((struct SCdiFinDevContSttUpdateFrmt *)(pThis->mpMsg))->devContState &= ~(0x03 << 0);
	((struct SCdiFinDevContSttUpdateFrmt *)(pThis->mpMsg))->devContState |= ((prdctStt & 0x03) << 0);
}
#define CCdiFinDevContSttUpdateMsg_GetTransitionState(apThis) ((UC)((((struct SCdiFinDevContSttUpdateFrmt*)((apThis)->mpMsg))->devContState >> 5) & 0x03))
																				// UC (CCdiFinDevContSttUpdateMsg*)
void CCdiFinDevContSttUpdateMsg_SetTransitionState(CCdiFinDevContSttUpdateMsg* pThis, UC trnstStt)
{
	((struct SCdiFinDevContSttUpdateFrmt *)(pThis->mpMsg))->devContState &= ~(0x03 << 5);
	((struct SCdiFinDevContSttUpdateFrmt *)(pThis->mpMsg))->devContState |= ((trnstStt & 0x03) << 5);
}


#endif // !defined(CCdiFinDevContSttUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
