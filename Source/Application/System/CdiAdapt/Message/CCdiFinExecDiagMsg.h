// File Name:	CCdiFinExecDiagMsg.h
// Description:	Header File of Execute Diagnostics Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.07
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinExecDiagMsg_h)
#define CCdiFinExecDiagMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDiagProgram.h"
#include "ECdiFinDiagCmd.h"

#include "SCdiFinExecDiagFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Execute Diagnostics Message ---
typedef struct /*CCdiFinExecDiagMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinExecDiagMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiFinExecDiagMsg_Constructor(CCdiFinExecDiagMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinExecDiagMsg_GetID(apThis) ((UC)eCdiFinMsgID_ExecDiag)			// UC (CCdiFinExecDiagMsg*)
#define CCdiFinExecDiagMsg_GetSize(apThis) (apThis->mSize)						// US (CCdiFinExecDiagMsg*)
#define CCdiFinExecDiagMsg_GetMessage(apThis) ((UC*)((apThis)->mpMsg))			// UC* (CCdiFinExecDiagMsg*)
void CCdiFinExecDiagMsg_SetMessage(CCdiFinExecDiagMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_ExecDiag;
}

#define CCdiFinExecDiagMsg_GetDiagProgram(apThis) (CF2_US(&(((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagProgram_MSB)))
																				// US (CCdiFinExecDiagMsg*)
#define CCdiFinExecDiagMsg_SetDiagProgram(apThis, aDiagProgram) (US_CF2(aDiagProgram, &((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagProgram_MSB)))
																				// void (CCdiFinExecDiagMsg*, US)
#define CCdiFinExecDiagMsg_GetDiagCommand(apThis) ((UC)((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagCommand)
																				// UC (CCdiFinExecDiagMsg*)
#define CCdiFinExecDiagMsg_SetDiagCommand(apThis, aDiagCommand) (((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagCommand = aDiagCommand)
																				// void (CCdiFinExecDiagMsg*, UC)
#define CCdiFinExecDiagMsg_GetDiagDataLength(apThis) ((UC)((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagDataLength)
																				// UC (CCdiFinExecDiagMsg*)
void CCdiFinExecDiagMsg_SetDiagDataLength(CCdiFinExecDiagMsg* pThis, UC diagDataLength)
{
	((struct SCdiFinExecDiagFrmt*)(pThis->mpMsg))->diagDataLength = diagDataLength;
	pThis->mSize = CDIFIN_EXEC_DIAG_MIN_SIZE + diagDataLength;
}

#define CCdiFinExecDiagMsg_GetDiagData(apThis) ((UC*)&(((struct SCdiFinExecDiagFrmt*)((apThis)->mpMsg))->diagData[0]))
																				// UC* (CCdiFinExecDiagMsg*)
void CCdiFinExecDiagMsg_ClearDiagData(CCdiFinExecDiagMsg *pThis)
{
	UC index;

	for ( index = 0; index < sizeof(((struct SCdiFinExecDiagFrmt*)(pThis->mpMsg))->diagData); index++ ){
		((struct SCdiFinExecDiagFrmt*)(pThis->mpMsg))->diagData[index] = 0x00;
	}
}


#endif // !defined(CCdiFinExecDiagMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
