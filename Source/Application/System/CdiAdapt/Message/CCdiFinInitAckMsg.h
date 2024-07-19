// File Name:	CCdiFinInitAckMsg.h
// Description:	Header File of Initialization Acknowledge Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinInitAckMsg_h)
#define CCdiFinInitAckMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinInitAckFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Initialization Acknowledge Message ---
static struct SCdiFinInitAckFrmt const cCdiFin_InitAckMsg = {
	eCdiFinMsgID_InitAck
};


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Initialization Acknowledge Message ---
typedef struct /*CCdiFinInitAckMsg*/
{
	UC*	mpMsg;
} CCdiFinInitAckMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinInitAckMsg_Constructor(CCdiFinInitAckMsg* pThis)
{
//	pThis->mpMsg = &cCdiFin_InitAckMsg;											// 未使用のため処理時間削減のためコメントアウト
}

#define CCdiFinInitAckMsg_GetID(apThis) ((UC)eCdiFinMsgID_InitAck)				// UC (CCdiFinInitAckMsg*)
#define CCdiFinInitAckMsg_GetSize(apThis) ((US)CDIFIN_INIT_ACK_MSG_SIZE)		// US (CCdiFinInitAckMsg*)
#define CCdiFinInitAckMsg_GetMessage(apThis) ((UC *)&cCdiFin_InitAckMsg)		// UC* (CCdiFinInitAckMsg*)
void CCdiFinInitAckMsg_SetMessage(CCdiFinInitAckMsg* pThis, UC* pMsg, US size)
{
	// No Action
}


#endif // !defined(CCdiFinInitAckMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
