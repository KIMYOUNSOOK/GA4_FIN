// File Name:	CCdiFinSetRecoveryMsg.h
// Description:	Header File of Set Recovery Request Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSetRecoveryMsg_h)
#define CCdiFinSetRecoveryMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinSetRecoveryFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Set Recovery Request Message ---
typedef struct /*CCdiFinSetRecoveryMsg*/
{
	UC*	mpMsg;
} CCdiFinSetRecoveryMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinSetRecoveryMsg_Constructor(CCdiFinSetRecoveryMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinSetRecoveryMsg_GetID(apThis) ((UC)eCdiFinMsgID_SetRecoveryReq)	// UC (CCdiFinSetRecoveryMsg*)
#define CCdiFinSetRecoveryMsg_GetSize(apThis) ((US)CDIFIN_SET_RECOVERY_MSG_SIZE)
																				// US (CCdiFinSetRecoveryMsg*)
#define CCdiFinSetRecoveryMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinSetRecoveryMsg*)
void CCdiFinSetRecoveryMsg_SetMessage(CCdiFinSetRecoveryMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_SetRecoveryReq;
}
#define CCdiFinSetRecoveryMsg_GetSetID(apThis) ((UC)(((struct SCdiFinSetRecoveryFrmt*)((apThis)->mpMsg))->setID))
																				// UC (CCdiFinSetRecoveryMsg*)
#define CCdiFinSetRecoveryMsg_SetSetID(apThis, aSetID) (((struct SCdiFinSetRecoveryFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinSetRecoveryMsg*, UC)


#endif // !defined(CCdiFinSetRecoveryMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
