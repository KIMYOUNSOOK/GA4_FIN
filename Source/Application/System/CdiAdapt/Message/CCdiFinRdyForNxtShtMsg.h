// File Name:	CCdiFinRdyForNxtShtMsg.h
// Description:	Header File of Ready For Next Sheet
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinRdyForNxtShtMsg_h)
#define CCdiFinRdyForNxtShtMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinPrepTime.h"

#include "SCdiFinRdyForNxtShtFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Ready For Next Sheet Message ---
typedef struct /*CCdiFinRdyForNxtShtMsg*/
{
	UC*	mpMsg;
} CCdiFinRdyForNxtShtMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinRdyForNxtShtMsg_Constructor(CCdiFinRdyForNxtShtMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinRdyForNxtShtMsg_GetID(apThis) ((UC)eCdiFinMsgID_ReadyForNextSheet)
																				// UC (CCdiFinRdyForNxtShtMsg*)
#define CCdiFinRdyForNxtShtMsg_GetSize(apThis) ((US)CDIFIN_RDY_FOR_NEXT_SHT_MSG_SIZE)
																				// US (CCdiFinRdyForNxtShtMsg*)
#define CCdiFinRdyForNxtShtMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinRdyForNxtShtMsg*)
void CCdiFinRdyForNxtShtMsg_SetMessage(CCdiFinRdyForNxtShtMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_ReadyForNextSheet;
}

#define CCdiFinRdyForNxtShtMsg_GetSetID(apThis) ((UC)((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->setID)
																				// UC (CCdiFinRdyForNxtShtMsg*)
#define CCdiFinRdyForNxtShtMsg_SetSetID(apThis, aSetID) (((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->setID = aSetID)
																				// void (CCdiFinRdyForNxtShtMsg*, UC)
#define CCdiFinRdyForNxtShtMsg_GetSheetID(apThis) ((UC)((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->sheetID)
																				// UC (CCdiFinRdyForNxtShtMsg*)
#define CCdiFinRdyForNxtShtMsg_SetSheetID(apThis, aSheetID) (((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->sheetID = aSheetID)
																				// void (CCdiFinRdyForNxtShtMsg*, UC)
#define CCdiFinRdyForNxtShtMsg_GetPrepTime(apThis) (CF2_US(&(((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->prepTime_Octet0)))
																				// US (CCdiFinRdyForNxtShtMsg*)
void CCdiFinRdyForNxtShtMsg_SetPrepTime(CCdiFinRdyForNxtShtMsg* pThis, US prepTime)
{
	US_CF2(prepTime, &(((struct SCdiFinRdyForNxtShtFrmt*)(pThis->mpMsg))->prepTime_Octet0));
}

#define CCdiFinRdyForNxtShtMsg_GetDispersionTime(apThis) ((UC)((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->disperTime)
																				// UC (CCdiFinRdyForNxtShtMsg*)
#define CCdiFinRdyForNxtShtMsg_SetDispersionTime(apThis, aDisperTime) (((struct SCdiFinRdyForNxtShtFrmt*)((apThis)->mpMsg))->disperTime = aDisperTime)
																				// void (CCdiFinRdyForNxtShtMsg*, UC)


#endif // !defined(CCdiFinRdyForNxtShtMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
