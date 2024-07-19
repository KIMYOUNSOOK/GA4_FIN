// File Name:	CSysFinSttChangeDiag.h
// Description:	Header File of Finisher State "Change Diag"
// Engineer:	Yuichi Hoshino
// Last Edit:	14.11.06
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSysFinSttChangeDiag_h
#define CSysFinSttChangeDiag_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CSysFinisherState.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSysFinSttChangeDiag_Constructor();

bool CSysFinSttChangeDiag_IsTransitionState();
bool CSysFinSttChangeDiag_CheckTransitionPossibility(EFinisherState nextState);
bool CSysFinSttChangeDiag_CheckExitCondition();
EFinisherState CSysFinSttChangeDiag_GetNextState();
void CSysFinSttChangeDiag_Entry(EFinisherState prevState);
bool CSysFinSttChangeDiag_ReceiveChangeDiagResponse(UC moduleID);
bool CSysFinSttChangeDiag_ReceiveDeviceActionResponse(UC moduleID, US eventID);



/////////////////////////////////////////////////////////////////////////////////////////
// Function Table Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Change Diag State ---
extern CSysFinisherState const gSys_FinSttChangeDiagMethod;


#endif // #ifndef CSysFinSttChangeDiag_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
