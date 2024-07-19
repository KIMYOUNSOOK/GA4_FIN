// File Name:	CCdiFinDiagRespMsg.h
// Description:	Header File of CCdiFinDiagRespMsg
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagRespMsg_h)
#define CCdiFinDiagRespMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDiagProgram.h"
#include "ECdiFinDiagCmd.h"
#include "ECdiFinDiagResult.h"

#include "SCdiFinDiagRespFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Diagnostics Response Message ---
typedef struct /*CCdiFinDiagRespMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinDiagRespMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinDiagRespMsg_Constructor(CCdiFinDiagRespMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinDiagRespMsg_GetID(apThis) ((UC)eCdiFinMsgID_DiagResp)			// UC (CCdiFinDiagRespMsg*)
#define CCdiFinDiagRespMsg_GetSize(apThis) ((apThis)->mSize)						// US (CCdiFinDiagRespMsg*)
#define CCdiFinDiagRespMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinDiagRespMsg*)
void CCdiFinDiagRespMsg_SetMessage(CCdiFinDiagRespMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_DiagResp;
}

#define CCdiFinDiagRespMsg_GetDiagProgram(apThis) ( ((US)(((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagProgram_MSB) << 8) | (US)(((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagProgram_LSB) )
																				// US (CCdiFinDiagRespMsg*)
#define CCdiFinDiagRespMsg_SetDiagProgram(apThis, aDiagProgram) ((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagProgram_MSB = (UC)((US)(aDiagProgram) >> 8); ((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagProgram_LSB = (UC)(aDiagProgram)
																				// void (CCdiFinDiagRespMsg*, US)
#define CCdiFinDiagRespMsg_GetDiagCommand(apThis) ((UC)((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagCommand)
																				// UC (CCdiFinDiagRespMsg*)
#define CCdiFinDiagRespMsg_SetDiagCommand(apThis, aDiagCommand) (((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagCommand = aDiagCommand)
																				// void (CCdiFinDiagRespMsg*, UC)
#define CCdiFinDiagRespMsg_GetDiagResult(apThis) ((UC)((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagResult)
																				// UC (CCdiFinDiagRespMsg*)
#define CCdiFinDiagRespMsg_SetDiagResult(apThis, aDiagResult) (((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagResult = aDiagResult)
																				// void (CCdiFinDiagRespMsg*, UC)
#define CCdiFinDiagRespMsg_GetDiagDataLength(apThis) ((UC)((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagDataLength)
																				// UC (CCdiFinDiagRespMsg*)
void CCdiFinDiagRespMsg_SetDiagDataLength(CCdiFinDiagRespMsg *pThis, UC diagDataLength)
{
	 ((struct SCdiFinDiagRespFrmt*)(pThis->mpMsg))->diagDataLength = diagDataLength;
	 pThis->mSize = CDIFIN_DIAG_RESP_MIN_SIZE + diagDataLength;
}

#define CCdiFinDiagRespMsg_GetDiagData(apThis) ((UC*)&(((struct SCdiFinDiagRespFrmt*)((apThis)->mpMsg))->diagData[0]))
																				// UC* (CCdiFinDiagRespMsg*)
void CCdiFinDiagRespMsg_ClearDiagData(CCdiFinDiagRespMsg *pThis)
{
	UC index;

	for ( index = 0; index < sizeof(((struct SCdiFinDiagRespFrmt*)(pThis->mpMsg))->diagData); index++ ){
		((struct SCdiFinDiagRespFrmt*)(pThis->mpMsg))->diagData[index] = 0x00;
	}
}


#endif // !defined(CCdiFinDiagRespMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
