// File Name:	CShtInitRequestWaitTime.h
// Description:	Header File of Sheet Initialize Request(Sheet Finish Request) Wait Time
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.23
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtInitRequestWaitTime_h
#define CShtInitRequestWaitTime_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "LIB_StopWatch.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Initialize Request Wait Time ---
typedef struct /*CShtInitRequestWaitTime*/
{
	CStopWatch mStopWatch;
	UC mState;
	UC mSheetNo;
} CShtInitRequestWaitTime;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtInitRequestWaitTime_Constructor(CShtInitRequestWaitTime *pThis);

void CShtInitRequestWaitTime_Reset(CShtInitRequestWaitTime *pThis);
void CShtInitRequestWaitTime_SetWaitSheet(CShtInitRequestWaitTime *pThis, UC sheetNo);
void CShtInitRequestWaitTime_StartWaiting(CShtInitRequestWaitTime *pThis, UC sheetNo);
UL CShtInitRequestWaitTime_GetWaitTime(CShtInitRequestWaitTime *pThis, UC sheetNo);


#endif // #ifndef CShtInitRequestWaitTime_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
