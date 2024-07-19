// File Name:	CCdiFinShtDelvToOutMsg.h
// Description:	Header File of Sheet Delivered To Output Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinShtDelvToOutMsg_h)
#define CCdiFinShtDelvToOutMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinShtDelvToOutFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Delivered To Output Message ---
typedef struct /*CCdiFinShtDelvToOutMsg*/
{
	UC*	mpMsg;
} CCdiFinShtDelvToOutMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinShtDelvToOutMsg_Constructor(CCdiFinShtDelvToOutMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinShtDelvToOutMsg_GetID(apThis) ((UC)eCdiFinMsgID_SheetDelvToOutput)
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetSize(apThis) ((US)CDIFIN_SHT_DELV_TO_OUT_MSG_SIZE)
																				// US (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinShtDelvToOutMsg*)
void CCdiFinShtDelvToOutMsg_SetMessage(CCdiFinShtDelvToOutMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_SheetDelvToOutput;
}
#define CCdiFinShtDelvToOutMsg_GetSetID(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->setID))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetSetID(apThis, aSetID) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinShtDelvToOutMsg*, UC)
#define CCdiFinShtDelvToOutMsg_GetSheetID(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->sheetID))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetDestID(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->destID))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetDestID(apThis, aDestID) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->destID = aDestID)
																				// void (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetBinSelect(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->binSlct))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetBinSelect(apThis, aBinSlct) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->binSlct = aBinSlct)
																				// void (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetSheetIntegrity(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->integrity))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetSheetIntegrity(apThis, aIntegrity) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->integrity = aIntegrity)
																				// void (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_GetOffsetPosition(apThis) ((UC)(((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->offsetPos))
																				// UC (CCdiFinShtDelvToOutMsg*)
#define CCdiFinShtDelvToOutMsg_SetOffsetPosition(apThis, aOffsetPos) (((struct SCdiFinShtDelvToOutFrmt*)((apThis)->mpMsg))->offsetPos = aOffsetPos)
																				// void (CCdiFinShtDelvToOutMsg*, UC)


#endif // !defined(CCdiFinShtDelvToOutMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
