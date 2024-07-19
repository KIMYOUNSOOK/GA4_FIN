// File Name:	CCdiFinDevContModeUpdateMsg.h
// Description:	Header File of Device Controller Mode Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContModeUpdateMsg_h)
#define CCdiFinDevContModeUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevContModeUpdtFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Controller Mode Update Message ---
typedef struct /*CCdiFinDevContModeUpdateMsg*/
{
	UC*	mpMsg;
} CCdiFinDevContModeUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevContModeUpdateMsg_Constructor(CCdiFinDevContModeUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinDevContModeUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevContModeUpdate)
																				// UC (CCdiFinDevContModeUpdateMsg*)
#define CCdiFinDevContModeUpdateMsg_GetSize(apThis) ((US)CDIFIN_DEV_CONT_MODE_UPDT_MSG_SIZE)
																				// US (CCdiFinDevContModeUpdateMsg*)
#define CCdiFinDevContModeUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)		// UC* (CCdiFinDevContModeUpdateMsg*)
void CCdiFinDevContModeUpdateMsg_SetMessage(CCdiFinDevContModeUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_DevContModeUpdate;

	// --- 未使用Bitのクリア ---
	((struct SCdiFinDevContModeUpdtFrmt*)(pThis->mpMsg))->devContMode &= ((UC)0x0F << 0);
}
#define CCdiFinDevContModeUpdateMsg_GetSysOpeMode(apThis) ((UC)(((struct SCdiFinDevContModeUpdtFrmt*)((apThis)->mpMsg))->devContMode & 0x0F))
																				// UC (CCdiFinDevContModeUpdateMsg*)
void CCdiFinDevContModeUpdateMsg_SetSysOpeMode(CCdiFinDevContModeUpdateMsg* pThis, UC sysOpeMode){
	((struct SCdiFinDevContModeUpdtFrmt*)(pThis->mpMsg))->devContMode &= ~(0x0F << 0);
	((struct SCdiFinDevContModeUpdtFrmt*)(pThis->mpMsg))->devContMode |= ((sysOpeMode & 0x0F) << 0);
}


#endif // !defined(CCdiFinDevContModeUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
