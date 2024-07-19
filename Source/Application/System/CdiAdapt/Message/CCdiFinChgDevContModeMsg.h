// File Name:	CCdiFinChgDevContModeMsg.h
// Description:	Header File of Change Device Controller Mode Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinChgDevContModeMsg_h)
#define CCdiFinChgDevContModeMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinSysOpeMode.h"

#include "SCdiFinChgDevContModeFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Change Device Capabilities Request Message ---
typedef struct /*CCdiFinChgDevContModeMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinChgDevContModeMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinChgDevContModeMsg_Constructor(CCdiFinChgDevContModeMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinChgDevContModeMsg_GetID(apThis) ((UC)eCdiFinMsgID_ChgDevContMode)// UC (CCdiFinChgDevContModeMsg*)
#define CCdiFinChgDevContModeMsg_GetSize(apThis) ((US)CDIFIN_CHG_DEV_CONT_MODE_MSG_SIZE)
																				// US (CCdiFinChgDevContModeMsg*)
#define CCdiFinChgDevContModeMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinChgDevContModeMsg*)
void CCdiFinChgDevContModeMsg_SetMessage(CCdiFinChgDevContModeMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_ChgDevContMode;

	((struct SCdiFinChgDevContModeFrmt *)(pThis->mpMsg))->devContMode &= 0x0F;
}

#define CCdiFinChgDevContModeMsg_GetSysOpeMode(apThis) ((UC)((((struct SCdiFinChgDevContModeFrmt*)((apThis)->mpMsg))->devContMode) & 0x0F))
																				// UC (CCdiFinChgDevContModeMsg*)
void CCdiFinChgDevContModeMsg_SetSysOpeMode(CCdiFinChgDevContModeMsg* pThis, UC sysOpeMode)
{
	((struct SCdiFinChgDevContModeFrmt *)(pThis->mpMsg))->devContMode &= 0xF0;
	((struct SCdiFinChgDevContModeFrmt *)(pThis->mpMsg))->devContMode |= (sysOpeMode & 0x0F);
}


#endif // !defined(CCdiFinChgDevContModeMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
