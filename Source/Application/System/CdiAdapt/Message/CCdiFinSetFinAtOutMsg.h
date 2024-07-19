// File Name:	CCdiFinSetFinAtOutMsg.h
// Description:	Header File of Set Finished At Output Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSetFinAtOutMsg_h)
#define CCdiFinSetFinAtOutMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinSetFinAtOutFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Set Finished At Output Message ---
typedef struct /*CCdiFinSetFinAtOutMsg*/
{
	UC*	mpMsg;
} CCdiFinSetFinAtOutMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinSetFinAtOutMsg_Constructor(CCdiFinSetFinAtOutMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinSetFinAtOutMsg_GetID(apThis) ((UC)eCdiFinMsgID_SetFinAtOutput)	// UC (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_GetSize(apThis) ((US)CDIFIN_SET_FIN_AT_OUT_MSG_SIZE)
																				// US (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinSetFinAtOutMsg*)
void CCdiFinSetFinAtOutMsg_SetMessage(CCdiFinSetFinAtOutMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_SetFinAtOutput;
}

#define CCdiFinSetFinAtOutMsg_GetSetID(apThis) ((UC)(((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->setID))
																				// UC (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_SetSetID(apThis, aSetID) (((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinSetFinAtOutMsg*, UC)
#define CCdiFinSetFinAtOutMsg_GetDestID(apThis) ((UC)(((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->destID))
																				// UC (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_SetDestID(apThis, aDestID) (((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->destID = aDestID)
																				// void (CCdiFinSetFinAtOutMsg*, UC)
#define CCdiFinSetFinAtOutMsg_GetBinSelect(apThis) ((UC)(((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->binSlct))
																				// UC (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_SetBinSelect(apThis, aBinSlct) (((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->binSlct = aBinSlct)
																				// void (CCdiFinSetFinAtOutMsg*, UC)
#define CCdiFinSetFinAtOutMsg_GetSetIntegrity(apThis) ((UC)(((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->integrity))
																				// UC (CCdiFinSetFinAtOutMsg*)
#define CCdiFinSetFinAtOutMsg_SetSetIntegrity(apThis, aIntegrity) (((struct SCdiFinSetFinAtOutFrmt*)((apThis)->mpMsg))->integrity = aIntegrity)
																				// void (CCdiFinSetFinAtOutMsg*, UC)

#endif // !defined(CCdiFinSetFinAtOutMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
