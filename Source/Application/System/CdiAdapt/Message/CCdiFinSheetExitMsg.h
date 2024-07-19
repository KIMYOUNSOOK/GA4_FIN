// File Name:	CCdiFinSheetExitMsg.h
// Description:	Header File of Sheet Exit Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinSheetExitMsg_h)
#define CCdiFinSheetExitMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinSheetExitFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Exit Message ---
typedef struct /*CCdiFinSheetExitMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinSheetExitMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinSheetExitMsg_Constructor(CCdiFinSheetExitMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinSheetExitMsg_GetID(apThis) ((UC)eCdiFinMsgID_SheetExit)
																				// UC (CCdiFinSheetExitMsg*)
#define CCdiFinSheetExitMsg_GetSize(apThis) ((US)CDIFIN_SHT_EXIT_MSG_SIZE)
																				// US (CCdiFinSheetExitMsg*)
#define CCdiFinSheetExitMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinSheetExitMsg*)
void CCdiFinSheetExitMsg_SetMessage(CCdiFinSheetExitMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_SheetExit;
}

#define CCdiFinSheetExitMsg_GetSetID(apThis) ((UC)((struct SCdiFinSheetExitFrmt*)((apThis)->mpMsg))->setID)
																				// UC (CCdiFinSheetExitMsg*)
#define CCdiFinSheetExitMsg_SetSetID(apThis, aSetID) (((struct SCdiFinSheetExitFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinSheetExitMsg*, UC)
#define CCdiFinSheetExitMsg_GetSheetID(apThis) ((UC)((struct SCdiFinSheetExitFrmt*)((apThis)->mpMsg))->sheetID)
																				// UC (CCdiFinSheetExitMsg*)
#define CCdiFinSheetExitMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinSheetExitFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinSheetExitMsg*, UC)


#endif // !defined(CCdiFinSheetExitMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
