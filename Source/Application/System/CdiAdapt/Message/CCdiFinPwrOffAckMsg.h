// File Name:	CCdiFinPwrOffAckMsg.h
// Description:	Header File of Power Off Acknowledge Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinPwrOffAckMsg_h)
#define CCdiFinPwrOffAckMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinPwrOffAckFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Power Off Acknowledge Message ---
static struct SCdiFinPwrOffAckFrmt const cCdiFin_PwrOffAckMsg = {
	eCdiFinMsgID_PwrOffAck
};


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Power Off Acknowledge Message ---
typedef struct /*CCdiFinPwrOffAckMsg*/
{
	UC*	mpMsg;
} CCdiFinPwrOffAckMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinPwrOffAckMsg_Constructor(CCdiFinPwrOffAckMsg* pThis)
{
//	pThis->mpMsg = &cCdiFin_InitReqMsg;											// 未使用のため処理時間削減のためコメントアウト
}

#define CCdiFinPwrOffAckMsg_GetID(apThis) ((UC)eCdiFinMsgID_PwrOffAck)			// UC (CCdiFinPwrOffAckMsg*)
#define CCdiFinPwrOffAckMsg_GetSize(apThis) ((US)CDIFIN_PWR_OFF_ACK_MSG_SIZE)	// US (CCdiFinPwrOffAckMsg*)
#define CCdiFinPwrOffAckMsg_GetMessage(apThis) ((UC *)&cCdiFin_PwrOffAckMsg)	// UC* (CCdiFinPwrOffAckMsg*)
void CCdiFinPwrOffAckMsg_SetMessage(CCdiFinPwrOffAckMsg* pThis, UC* pMsg, US size)
{
	// No Action
}


#endif // !defined(CCdiFinPwrOffAckMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
